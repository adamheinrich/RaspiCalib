#ifndef RASPI_VIDEO_CAPTURE_H
#define RASPI_VIDEO_CAPTURE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat RaspiVideoCapture_pop(void);
void RaspiVideoCapture_start(void);

class RaspiVideoCapture
{
	cv::VideoCapture *m_vc;
	bool m_use_raspivid = false;

public:
	RaspiVideoCapture()
	{
	}

	~RaspiVideoCapture()
	{
		delete m_vc;
	}

	bool open(int index)
	{
		m_use_raspivid = true;
		RaspiVideoCapture_start();
	}

	bool open(const cv::String &filename)
	{
		m_use_raspivid = false;
		m_vc = new cv::VideoCapture();
		m_vc->open(filename);
	}

	bool isOpened() const
	{
		if (m_use_raspivid)
			return true;
		else
			return m_vc->isOpened();
	}

	RaspiVideoCapture& operator >> (cv::Mat& image)
	{
		image = RaspiVideoCapture_pop();
		return *this;
	}
};

#endif
