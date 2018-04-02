#include "ORBMatcher.h"

namespace grl {

bool
ORBMatcher::addGesture(const std::string& name, const std::string& filePath)
{
	cv::Mat image = cv::imread(filePath);
	if (!image.data) return false;

	addGesture(name, image);
	return true;
}

void
ORBMatcher::addGesture(const std::string& name, cv::Mat image)
{
	cv::Mat desc;
	std::vector<cv::KeyPoint> kp;

	// Get keypoints and descriptor
	_detector->detectAndCompute(image, cv::noArray(), kp, desc);

	auto it = _gestures.insert(std::make_pair(name, Gesture()));

	Gesture& gest = it.first->second;

	// If element is new assign first image of the gesture
	if (it.second) {
		std::cout << "Adding new gesture: " << name << std::endl;
		gest.image = image;
		gest.matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
	} else
		std::cout << "Extending gesture: " << name << std::endl;

	gest.matcher->add(desc);
}

void 
ORBMatcher::learn()
{
}

ORBMatcher::Match
ORBMatcher::matchBestGesture(cv::Mat& image)
{
	std::vector<Match> allMatches;
	// Check with every gesture
	for (auto it = _gestures.begin(); it != _gestures.end(); ++it) {
		// Data for current frame
		std::vector<cv::KeyPoint> kp;
		cv::Mat desc;
		// Get keypoints and descriptor of image
		_detector->detectAndCompute(image, cv::noArray(), kp, desc);

		// Vectors for results
		std::vector<std::vector<cv::DMatch> > matches;
		std::vector<cv::DMatch> filtered;

		it->second.matcher->knnMatch(desc, matches, 4);
		for (unsigned i = 0; i < matches.size(); i++) {
			if (matches[i][0].distance < _ratioT * matches[i][1].distance) {
				filtered.push_back(matches[i][1]);
			}
		}

		// Update record
		allMatches.push_back(Match{it->first, static_cast<double>(filtered.size()), it->second.image});
	}

	// Find best match
	auto bestMatch = std::max_element(allMatches.begin(), allMatches.end(),
        [](const Match& tr1, const Match& tr2)
	    { return tr1.matches < tr2.matches; });

	return bestMatch != allMatches.end() ? *bestMatch : Match();
}

}