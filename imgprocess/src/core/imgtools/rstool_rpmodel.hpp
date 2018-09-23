//
// Created by penglei on 18-9-22.
//
// 用于 “读(read)-处理(process)” 模型的算法（rp-model）

#ifndef IMGPROCESS_RSTOOL_RPMODEL_HPP
#define IMGPROCESS_RSTOOL_RPMODEL_HPP

#include "rstool_threadpool.h"

namespace RSTool {

    namespace Mp {

        template <typename T>
        class MpRPModel {
        public:

            /**
             * 用于“读-处理”算法模型
             * @param infile            输入待处理的文件
             * @param specDims          指定待处理的光谱范围
             * @param intl              指定在内存中以何种方式组织数据
             * @param blkSize           指定分块大小，默认 128
             * @param readQueueMaxSize  指定读缓冲区中的队列最大大小，默认 16
             * @param readThreadsCount  指定并行读取文件时的线程个数，默认 4
             */
            MpRPModel(const std::string &infile, const SpectralDimes &specDims,
                    Interleave intl = Interleave::BIP, int blkSize = 128,
                    int readQueueMaxSize = 16, int readThreadsCount = 4)
                    : infile_(infile), specDims_(specDims), intl_(intl),
                    blkSize_(blkSize), readQueueMaxSize_(readQueueMaxSize),
                    readThreadsCount_(readThreadsCount_),
                    mpRead_(infile_, specDims_, intl_, blkSize_, readThreadsCount_) {

                MpGDALRead<T>::readQueueMaxSize_ = readQueueMaxSize;

            } // end MpGdalIO

        public:
            // 为每个消费者线程指定处理函数（因为每个线程所需要的参数可能不一样）
            template <class Fn, class... Args>
            void emplaceTask(Fn &&fn, Args &&... args) {
                consumerTasks_.emplace_back(std::bind(std::forward<Fn>(fn), std::placeholders::_1,
                        std::forward<Args>(args)...));
            }

            // 启动所有消费者线程，会阻塞调用者线程，直到所有消费者线程处理完成
            void run() {
                assignWorkload();

                for (int i = 0; i < consumerCount_; i++) {
                    consumerThreads_.emplace_back(std::thread(&MpRPModel<T>::consumerTask,
                            this, consumerTasks_[i], tasks_[i]));
                }

                // todo 如果不需要和主线程进行同步，是否可以分离线程？？？
                for (auto &consumer : consumerThreads_) {
                    consumer.join();
                }
            }

        private:

            void assignWorkload() {
                GDALAllRegister();

                GDALDataset *ds = (GDALDataset*)GDALOpen(infile_.c_str(), GA_ReadOnly);
                if (ds == nullptr) {
                    throw std::runtime_error("GDALDataset open faild.");
                }

                int imgXSize = ds->GetRasterXSize();
                int imgYSize = ds->GetRasterYSize();
                int imgBandCount = ds->GetRasterCount();

                int xNUms = (imgXSize + blkSize_ - 1) / blkSize_;
                int yNUms = (imgYSize + blkSize_ - 1) / blkSize_;
                blkNums_ = xNUms*yNUms;

                consumerCount_ = NumThreads(1, blkNums_);

                // 分割文件(以方形块为单位)
                std::vector<SpatialDims> spatDims;
                for (int i = 0; i < imgYSize; i += blkSize_) {
                    int yBlockSize = blkSize_;
                    if (i + blkSize_ > imgYSize) // 最下面的剩余块
                        yBlockSize = imgYSize - i;

                    for (int j = 0; j < imgXSize; j += blkSize_) {
                        int xBlockSize = blkSize_;
                        if (j + blkSize_ > imgXSize) // 最右侧的剩余块
                            xBlockSize = imgXSize - j;

                        spatDims.emplace_back(SpatialDims(j, i, xBlockSize, yBlockSize));
                    } // end row
                } // end col

                // 分配 IO线程 工作量
                int perThreadYBlkNums = yNUms / readThreadsCount_;
                int leftYNums = yNUms % readThreadsCount_;
                int size = perThreadYBlkNums*xNUms; // 每一个线程需处理的任务量（块数）
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < readThreadsCount_; j++) {
                        mpRead_.enqueue(j, spatDims[j*perThreadYBlkNums+i]);
                    }
                }

                // 将剩余的块交给最后一个线程池 IO 处理
                int leftNums = leftYNums*xNUms;
                for (int i = 0; i < leftNums; i++) {
                    mpRead_.enqueue(readThreadsCount_-1, spatDims[readThreadsCount_*size+i]);
                }

                // 每个 消费者线程 需要处理的任务量（块数）
                int perNums = blkNums_ / consumerCount_;
                int leftsNums = blkNums_ % consumerCount_;

                tasks_.resize(consumerCount_);
                for (auto &tasks : tasks_) {
                    tasks = perNums;
                }
                tasks_[consumerCount_-1] += leftsNums; // 剩余的任务全部交给最后一个消费者去做

            } // end assignWorkload()

            void consumerTask(std::function<void(DataChunk<T> &)> &&funcCore, int tasks) {

                auto func = std::forward<std::function<void(DataChunk<T> &)>>(funcCore);

                DataChunk<T> data(0,0,1,1,1); // 临时构造一个数据块
                for (int i = 0; i < tasks; ++i) {

                    {
                        // 如果缓冲区中没有数据,则等待数据的到来
                        std::unique_lock<std::mutex> lk(MpGDALRead<T>::mutexReadQueue_);
                        while (MpGDALRead<T>::readQueue_.empty()) {
                            MpGDALRead<T>::condReadQueueNotEmpty_.wait(lk);
                        }

                        data = std::move(MpGDALRead<T>::readQueue_.front());
                        MpGDALRead<T>::readQueue_.pop();
                    }
                    MpGDALRead<T>::condReadQueueNotFull_.notify_all();

                    // TODO 核心操作，由用户实现
                    // 对于“读-处理”模型算法，函数内部不涉及写数据
                    func(data);
                }
            } // end consumerTask()

        private:
            std::string infile_;
            SpectralDimes specDims_;
            Interleave intl_;

            int blkSize_;
            int readQueueMaxSize_;
            int readThreadsCount_;

            MpGDALRead<T> mpRead_;

        private:
            int blkNums_;
            std::vector<int> tasks_;
        private:
            int consumerCount_; // 消费者线程数量
            std::vector<std::thread> consumerThreads_;   // 消费者线程（块数据处理线程）

            // 每个消费者线程所处理的核心函数对象
            // 可以从主线程中接收不同的参数（主要是为了将主线程的任务并行化）
            // 块处理的核心功能，由使用者负责实现，可传入可调用对象：
            //      lambda、成员函数、全局函数、函数对象以及bind表达式等
            std::vector< std::function<void(DataChunk<T> &)>> consumerTasks_;
        };


    } // namespace Mp

} // namespace RSTool

#endif //IMGPROCESS_RSTOOL_RPMODEL_HPP
