#include "HistogramGray.h"

using namespace cv;

namespace grl {

Mat
HistogramGray::getHistogramAsImage(const Mat & hist, int scale)
{
	int size = DEFAULT_SIZE * scale;
	int sizeh = cvRound(size * 0.9);
	int binWidth = cvRound((float)size / _histSize[0]);
	Mat histImage = Mat(size, size, CV_8UC3, Scalar::all(255));
	double max;

	minMaxLoc(hist, nullptr, &max);
	for (int i = 0; i < _histSize[0]; i++) {
		float binVal = hist.at<float>(i);
		int val = cvRound(binVal / max*sizeh);
		int x = binWidth*i;
		rectangle(histImage, Point(x, size), Point(x + binWidth, size - val),
				  Scalar::all(0),
				  CV_FILLED);
	}

	return histImage;
}

void
HistogramGray::init(int bins)
{
	_channels.resize(1);
	_channels[0] = 0;
	_dims = 1;
	_histSize.resize(1);
	_histSize[0] = bins;
	_range.resize(2);
	_range[0] = COLOR_MIN;
	_range[1] = COLOR_MAX;
	_ranges.resize(1);
	_ranges[0] = &_range[0];
}


std::vector<Peakeness>
HistogramGray::getPeakenessList(const cv::Mat& hist1)
{
	std::vector<Peakeness> peaks;
	cv::MatConstIterator_<float> it,
		pit = hist1.begin<float>(),
		end = hist1.end<float>();
	int i = 0;
	bool peakActive = false;
	bool growing = false;
	bool pushed = false;
	Peakeness peak;

	for (it = hist1.begin<float>() + 1; it != end; ++it, ++pit, ++i) {
		float next = *it;
		float val = *pit;

		if (peakActive) {
			peak.width++;
			peak.value += i*val;

			if (growing) {
				/* Jeœli przestaliœmy rosn¹æ, to tutaj mamy max */
				if (next < val) {
					peak.height = val;
					peak.maxPos = i;
					growing = false;
				}
			} else {
				/* Przypadek, gdy od razu jest odbicie */
				if (next > val) {
					peak.colRight = i + 1;
					peaks.push_back(peak);
					pushed = false;
					/* Inicjujemy nastêpny pik */
					peak = {0, 0, 0, 0, i + 1, 0};
					growing = true;
				}
				/* Przypadek, gdy pik siê koñczy i czekamy na nastepny */
				else if (next == val) {
					peak.colRight = i + 1;
					peaks.push_back(peak);
					pushed = true;
					/* Nie musimy resetowaæ piku, zrobi to else poni¿ej */
					growing = false;
					peakActive = false;
				}
			}
		} else if (next > val) {
			/* Znajdujemy pocz¹tek piku */
			peak = {1, val, i, i*val, i, i+1};
			pushed = false;
			growing = true;
			peakActive = true;
		}
	}
	if (!pushed && peak.width != 0) {
		peak.colRight = i + 1;
		peaks.push_back(peak);
	}
	return peaks;
}


Peakeness
HistogramGray::getPeakeness(const cv::Mat& hist1, int position)
{
	cv::MatConstIterator_<float> it,
		pit = hist1.begin<float>(),
		end = hist1.end<float>();
	int i = 0;
	int peakNum = 0;
	bool peakActive = false;
	bool growing = false;
	Peakeness peak{0, 0, 0, 0, 0, 0};

	for (it = hist1.begin<float>() + 1; it != end; ++it, ++pit, ++i) {
		float next = *it;
		float val = *pit;

		if (peakActive) {

			if (peakNum == position) {
				peak.width++;
				peak.value += i*val;
			}

			if (growing) {
				/* Jeœli przestaliœmy rosn¹æ, to tutaj mamy max */
				if (next < val) {
					if (peakNum == position) {
						peak.height = val;
						peak.maxPos = i;
					}
					growing = false;
				}
			} else {
				/* Przypadek, gdy od razu jest odbicie */
				if (next > val) {
					if (peakNum == position) {
						peak.colRight = i + 1;
						break;
					}
					peakNum++;
					if (peakNum == position)
						peak.colLeft = i + 1;
					growing = true;
				}
				/* Przypadek, gdy pik siê koñczy i czekamy na nastepny */
				else if (next == val) {
					if (peakNum == position) {
						peak.colRight = i + 1;
						break;
					}
					growing = false;
					peakActive = false;
				}
			}
		} else if (next > val) {
			peakNum++;
			/* Znajdujemy pocz¹tek piku */
			if (peakNum == position)
				peak = {1, val, i, i*val, i, i + 1};
			growing = true;
			peakActive = true;
		}
	}

	/* Jeœli nie doszliœmy do koñca... */
	if (peak.colRight < peak.colLeft)
		peak.colRight = i + 1;

	return peak;
}

}
