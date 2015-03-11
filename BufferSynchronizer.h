#pragma once
#include "Buffer.h"

#include <boost/signal.hpp>
#include <thread>
#include <condition_variable>
#include <mutex>

template < class BufferDataType >
class BufferSynchronizer
{
public:
	BufferSynchronizer();
	~BufferSynchronizer();

	void updateThreadFunc();

	void onApplicationQuit();
	
	void stopPlayback();

	void signalDataOfBufferWithIndexIsReady(int index);

	void setBuffer(std::vector<std::shared_ptr<Buffer<BufferDataType>>> buffers, int numOfFilesToRead);
	
	boost::signal<void(void)> playbackFinished;

	boost::signal<void(std::vector<pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr> cloud)> cloudsUpdated;


	
private:
	void BufferSynchronizer::printMessage(std::string msg);
	int m_numOfFilesToRead;

	std::vector<std::pair<std::shared_ptr<Buffer<BufferDataType>>, bool>> m_bufferWithReadyState;
	std::mutex m_printMutex;
	std::mutex m_updateBuffer;
	std::condition_variable m_isDataAvailableConditionVariable;
	//std::shared_ptr<std::mutex> m_printMutex;
	//std::shared_ptr<std::mutex> m_updateBuffer;
	//std::shared_ptr<std::condition_variable> m_isDataAvailableConditionVariable;
	bool m_isDataAvaiable;
	bool m_playbackActive;
	bool m_isRunning;
};

