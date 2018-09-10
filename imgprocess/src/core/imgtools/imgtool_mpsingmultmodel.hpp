//
// Created by penglei on 18-9-10.
//

#ifndef IMGPROCESS_IMGTOOL_MPSINGMULTMODEL_HPP
#define IMGPROCESS_IMGTOOL_MPSINGMULTMODEL_HPP

#include "imgtool_common.hpp"
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class GDALDataset;

namespace ImgTool {

    namespace Mp {

        template <typename T>
        struct DataBufferQueue {
            std::vector<ImgBlockData<T>> items_;

            size_t readPos_;
            size_t  writePos_;

            std::mutex mutex_;

            std::condition_variable bufNotFull_;
            std::condition_variable bufNotEmpty_;

            size_t consumedItemCount_;
            std::mutex mutexConsumedItemCount_;

            int produceItemCount_;
        };


        // ImgTool::ImgBlockProcess<float> blockProcess(poSrcDS, 128, ImgTool::ImgBlockType::IBT_SQUARE);
        template <typename T>
        class MpSingleMultiModel {
        public:
            // 全波段处理
            MpSingleMultiModel(int consumeCount, int bufItemCount,
                               GDALDataset *dataset,
                               int blockSize = 8,
                               ImgBlockType blockType = IBT_LINE,
                               ImgInterleaveType dataInterleave = IIT_BIP)
                               : readFunc_(dataset){
                consumeCount_ = consumeCount;
                bufItemCount_ = bufItemCount;

                imgDataset_ = dataset;
                imgBandCount_ = imgDataset_->GetRasterCount();
                imgXSize_ = imgDataset_->GetRasterXSize();
                imgYSize_ = imgDataset_->GetRasterYSize();
                blkSize_ = blockSize;
                blkType_ = blockType;
                dataInterleave_ = dataInterleave;

            }

            virtual ~MpSingleMultiModel() {

            }

            // 读数据线程 "main()"
            void producerTask() {

                switch (blkType_) {
                    case ImgBlockType::IBT_LINE :
                    {
                        int blockNums = imgYSize_ / blkSize_;
                        int leftLines = imgYSize_ % blkSize_;

                        // 处理完整块
                        for (int i = 0; i < blockNums; i++) {
                            produceBlockData(0, i*blkSize_, imgXSize_, blkSize_);
                        }

                        // 处理剩余的最后一块（非完整块）
                        if (leftLines > 0) {
                            produceBlockData(0, blockNums*blkSize_, imgXSize_, leftLines);
                        }

                        break;
                    }

                    case ImgBlockType::IBT_SQUARE :
                    {
                        for (int i = 0; i < imgYSize_; i += blkSize_) {
                            for (int j = 0; j < imgXSize_; j += blkSize_) {
                                int xBlockSize = blkSize_;
                                int yBlockSize = blkSize_;

                                // 如果最下面和最右面的块不够 m_blockSize, 剩下多少读多少
                                if (i + blkSize_ > imgYSize_) // 最下面的剩余块
                                    yBlockSize = imgYSize_ - i;
                                if (j + blkSize_ > imgXSize_) // 最右侧的剩余块
                                    xBlockSize = imgXSize_ - j;

                                produceBlockData(j, i, xBlockSize, yBlockSize);
                            }
                        }

                        break;
                    }
                }// end switch
            }

            // 生产块数据
            void produceBlockData(int xOff, int yOff, int xSize, int ySize) {
                std::unique_lock<std::mutex> lock(bufQueue_.mutex_);

                while ( ((bufQueue_.writePos_ + 1) % bufItemCount_)
                    == bufQueue_.readPos_ ) {
                    (bufQueue_.bufNotFull_).wait(lock);
                }

                // todo 读取数据
                // $1 begin 从文件中读取一块数据
                bufQueue_.items_[bufQueue_.writePos_].updateSpatial(
                        xOff, yOff, xSize, ySize);
                readFunc_(bufQueue_.items_[bufQueue_.writePos_]);
                // $1 end

                bufQueue_.writePos_++;
                if (bufQueue_.writePos_ == bufItemCount_)
                    bufQueue_.writePos_ = 0;

                bufQueue_.bufNotEmpty_.notify_all();
                lock.unlock();
            }

            // 块数据处理线程 “main()”
            // 可能需要向每个处理线程传递不同的参数
            void consumerTask(std::function<void()> &&funcProcessDataCore) {
                bool readyToExit = false;

                // 用于从缓冲区中复制一块数据，进行独立处理（独立于线程）
                ImgBlockData<T> data;
                while (true) {

                    std::unique_lock<std::mutex> lock(bufQueue_.mutexConsumedItemCount_);
                    if (bufQueue_.consumedItemCount_ < bufQueue_.produceItemCount_) {

                        consumeBlockData(data); // 从缓冲区中复制一块数据
                        ++bufQueue_.consumedItemCount_;
                        lock.unlock();

                        // todo 处理每一块数据的核心函数，是否需要“完美转型”？
                        std::forward<std::function<void()>>(funcProcessDataCore)();

                    } else {
                        readyToExit = true;
                    }

                    if (readyToExit) break;

                }// end while
            }

            void consumeBlockData(ImgBlockData<T> &data) {
                std::unique_lock<std::mutex> lock(bufQueue_.mutex_);

                while ( bufQueue_.writePos_ == bufQueue_.readPos_ ) {
                    bufQueue_.bufNotEmpty_.wait(lock);
                }

                // $1 begin 从缓冲区中取走一个数据
                data = bufQueue_.items_[bufQueue_.readPos_];
                // $1 end

                bufQueue_.readPos_++;
                if (bufQueue_.readPos_ >= bufItemCount_)
                    bufQueue_.readPos_ = 0;

                bufQueue_.bufNotFull_.notify_all();
                lock.unlock();
            }

            template <class Fn, class... Args>
            void addProcessBlockData(Fn &&fn, Args &&... args) {
                // todo 是否需要智能指针？
//                auto consumer = std::make_shared< std::function<void()> >(
//                        std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...)
//                        );
//               //consumeTasks_.emplace_back([consumer](){(*consumer)();});

                auto consumer = std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...);
                consumeTasks_.emplace_back([consumer](){(consumer)();});
            }

        public:
            void run() {
                // 初始化相关信息
                if (blkType_ == IBT_LINE) {
                    bufQueue_.produceItemCount_ = imgYSize_ / blkSize_;
                    int leftLines = imgYSize_ % blkSize_;
                    if (leftLines > 0)  bufQueue_.produceItemCount_++;
                } else {
                    int xNums = imgXSize_ / blkSize_;
                    int yNums = imgYSize_ / blkSize_;
                    int xLeftLines = imgXSize_ % blkSize_;
                    int yLeftLines = imgYSize_ % blkSize_;

                    if (xLeftLines > 0) xNums++;
                    if (yLeftLines > 0) yNums++;

                    bufQueue_.produceItemCount_ = xNums*yNums;
                }

                bufQueue_.readPos_ = 0;
                bufQueue_.writePos_ = 0;
                bufQueue_.consumedItemCount_ = 0;

                // 创建缓冲区空间
                if (blkType_ == IBT_LINE) {
                    for (int i = 0; i < bufItemCount_; i++) {
                        bufQueue_.items_.emplace_back(
                                ImgBlockData<T>(ImgSpatialSubset(),
                                                ImgSpectralSubset(imgBandCount_),
                                                imgXSize_,
                                                blkSize_,
                                                dataInterleave_));
                    }
                } else {
                    for (int i = 0; i < bufItemCount_; i++) {
                        bufQueue_.items_.emplace_back(
                                ImgBlockData<T>(ImgSpatialSubset(),
                                                ImgSpectralSubset(imgBandCount_),
                                                blkSize_,
                                                blkSize_,
                                                dataInterleave_));
                    }
                }


                std::thread producer(&MpSingleMultiModel<T>::producerTask, this);

                for (int i = 0; i < consumeCount_; i++) {
                    consumeThreads_.emplace_back(std::thread(&MpSingleMultiModel<T>::consumerTask,
                            this, consumeTasks_[i]));
                }

                producer.join();
                for (int i = 0; i < consumeCount_; i++) {
                    consumeThreads_[i].join();
                }
            }

            const DataBufferQueue<T>& bufQueue() const { return bufQueue_; }

        protected:
            GDALDataset *imgDataset_;
            int imgBandCount_;
            int imgXSize_;
            int imgYSize_;

        private:
            int blkSize_;
            ImgBlockType blkType_;
            ImgInterleaveType dataInterleave_;
            ImgSpectralSubset spectralSubset_;

        private:
            ImgBlockData<T> blkData_;

        private:
            DataBufferQueue<T> bufQueue_;
            int consumeCount_;
            int bufItemCount_;

        private:
            ImgBlockDataRead<T> readFunc_;

        private:
            std::vector<std::thread> consumeThreads_;
            std::vector<std::function<void()>> consumeTasks_;
        };



    } // namespace Mp

} // namespace ImgTool

#endif //IMGPROCESS_IMGTOOL_MPSINGMULTMODEL_HPP