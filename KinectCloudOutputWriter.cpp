#include "KinectCloudOutputWriter.h"
#include <pcl/io/ply_io.h>


KinectCloudOutputWriter::KinectCloudOutputWriter() :
	m_running(false),
	m_notified(false),
	m_cloudCount(0),
	m_clouds(),
	m_writerThreads(),
	m_updateThreads()
{
	//startWritingClouds();
}


KinectCloudOutputWriter::~KinectCloudOutputWriter()
{
	for (auto& thread : m_updateThreads){
		thread.join();
	}
	m_running = false;
	for (auto& thread : m_writerThreads){
		thread.join();
	}
}

void KinectCloudOutputWriter::updateCloudThreated(pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud)
{
	m_updateThreads.push_back(std::thread(&KinectCloudOutputWriter::pushCloud, this, cloud));
}

void KinectCloudOutputWriter::pushCloud(pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloudToPush)
{
	/*if (!m_running){
		return;
	}*/
	//if (!m_running){
	//	startWritingClouds();
	//}
	std::unique_lock<std::mutex> cloudLocker(m_lockCloud);
	if (m_cloudCount > 100 || !m_running){
		return;
	}
	//PointCloudMeasurement cloudMeasurement{ cloudToPush, m_cloudCount };	
	PointCloudMeasurement cloudMeasurement;
	cloudMeasurement.cloud = cloudToPush;
	cloudMeasurement.index = m_cloudCount;
	m_clouds.push(cloudMeasurement);
	m_cloudCount++;
	if (m_cloudCount == 100){
		m_running = false;
	}
	//m_notified = true;
	m_checkCloud.notify_one();
}

void KinectCloudOutputWriter::startWritingClouds()
{
	const int threadsToStartCount = 5;
	m_running = true;
	for (int i = 0; i < threadsToStartCount; i++){
		m_writerThreads.push_back(std::thread(&KinectCloudOutputWriter::writeCloudToFile, this, i));
	}
}

void KinectCloudOutputWriter::writeCloudToFile(int index)
{
	while (m_running)
	{
		std::unique_lock<std::mutex> cloudLocker(m_lockCloud);
		//while (!m_notified){
			m_checkCloud.wait(cloudLocker);
		//}
		

		if (!m_clouds.empty())
		{
			auto cloudMeasurement = m_clouds.front();
			std::stringstream fileName;
			fileName << "Cloud_" << cloudMeasurement.index << "_" << index << ".ply";
			pcl::io::savePLYFile(fileName.str(), *cloudMeasurement.cloud, false);
			m_clouds.pop();
		}
		m_notified = false;
	}
}