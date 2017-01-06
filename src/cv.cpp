#include "cv.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <sys/time.h>

#include "opencv2/core/utility.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "cv_img.h"
#include "debug.h"

#include "RaspiVideoCapture.h"

using namespace cv;
using namespace std;

static struct {
	int width;
	int height;
} m_img;

static pthread_t m_thread;

static cv_img *m_cv_img;
static pthread_mutex_t m_img_mutex;
static pthread_cond_t m_img_cond;

static volatile bool m_initialized;

static suseconds_t microseconds()
{
	static struct timeval tv;
	gettimeofday(&tv, NULL);

	return ((tv.tv_sec * 1000000UL) + tv.tv_usec);
}

static void *process_thread(void *ptr)
{
	const char *rargv[] = { "raspivid", "-md", "4", "-w", "480", "-h",
				"480", "-fps", "30", "-t", "0", "-n", "-o",
				"/dev/null", "-x", "/dev/null", "-r",
				"/dev/null", "-rf", "gray" };
	int rargc = sizeof(rargv)/sizeof(rargv[0]);

	RaspiCV_main(rargc, rargv);
}

Mat RaspiVideoCapture_pop(void)
{
	static Mat img_rgb;
	static bool first = true;

	if (first) {
		img_rgb.create(m_img.height, m_img.width, CV_8UC3);
		first = false;
	}

	pthread_mutex_lock(&m_img_mutex);

	while (m_cv_img == NULL)
		pthread_cond_wait(&m_img_cond, &m_img_mutex);
	cvtColor(m_cv_img->mat(), img_rgb, CV_GRAY2RGB);

	pthread_mutex_unlock(&m_img_mutex);

	return img_rgb;
}

void RaspiVideoCapture_start(void)
{
	pthread_mutex_init(&m_img_mutex, NULL);
	pthread_cond_init(&m_img_cond, NULL);

	/* Start thread: */
	int rc = pthread_create(&m_thread, NULL, process_thread, NULL);
	if (rc)
		ERR("Unable to create thread: " << rc);
}

void cv_init(int width, int height, int fps, int fmt)
{
	DBG("cv_init(" << width << ", " << height << ", " << fps << ")");

	if (fmt != 3) {
		ERR("Format " << fmt << " is not grayscale");
		return;
	}

	m_img.width = width;
	m_img.height = height;

	m_initialized = true;
}

void cv_process_img(uint8_t *p_buffer, int length, int64_t timestamp)
{
	static int64_t prev_timestamp;
	suseconds_t t1, t2;

	if (!m_initialized)
		return;

	if (length != m_img.width*m_img.height) {
		ERR("Wrong img length: " << length);
		m_initialized = false;
		return;
	}

	t1 = microseconds();

	pthread_mutex_lock(&m_img_mutex);
	m_cv_img = new cv_img(p_buffer, m_img.width, m_img.height, t1);
	pthread_cond_signal(&m_img_cond);
	pthread_mutex_unlock(&m_img_mutex);

	t2 = microseconds();

	DBG("cv_process_img(p_buffer, " << length << ") [dts "
	    << (timestamp-prev_timestamp) << "]: " << (t2-t1) << " us");

	prev_timestamp = timestamp;
}

void cv_process_imv(uint8_t *p_buffer, int length, int64_t timestamp)
{
}

void cv_close(void)
{
	DBG("cv_close()");
	pthread_mutex_destroy(&m_img_mutex);
	pthread_cond_destroy(&m_img_cond);
	m_initialized = false;
}
