#include "CNNBlurDetector_UT.hpp"
#include <gtest/gtest.h>
#include "ModelConfig.hpp"
#include "CNNBlurDetector.hpp"
#include <memory>
#include <filesystem>


class CNNBlurDetectorTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		Py_Initialize();

	}

	void TearDown() override
	{
		Py_Finalize();
	}
};


TEST_F(CNNBlurDetectorTest, invalidConfig)
{

	boost::python::dict dict{};
	const char* imgPath = "INVALID";
	::testing::internal::CaptureStdout();

	BlurDetector::ModelConfig config{ imgPath, dict };
	ASSERT_EQ(config.isValid(), false);
	BlurDetector::CNNBlurDetector detector{ config };
	ASSERT_EQ(detector.classify(), BlurDetector::ResultTypes[BlurDetector::PROCESSING_ERROR]);
	ASSERT_EQ(::testing::internal::GetCapturedStdout(), "Provided invalid config\n");

}

TEST_F(CNNBlurDetectorTest, visualizationWithoutRunningFirstClassify)
{

	boost::python::dict dict{};
	const char* imgPath = "INVALID";
	const char* visualizationPath = "INVALID";

	BlurDetector::ModelConfig config{ imgPath, dict };
	ASSERT_EQ(config.isValid(), false);
	BlurDetector::CNNBlurDetector detector{ config };


	::testing::internal::CaptureStdout();
	detector.createImageWithCropsClassification(visualizationPath);
	ASSERT_EQ(::testing::internal::GetCapturedStdout(), "Cannot create image with classified crops without running classify first\n");

}


TEST_F(CNNBlurDetectorTest, regularScenarioWithoutVisualization)
{

	boost::python::dict dict;
	const char* imgPath = "../../files/blurred.jpg";
	dict["modelPath"] = "../../files/trainedModel.pt";
	dict["thresh"] = "0.5";

	BlurDetector::ModelConfig config{ imgPath, dict };
	ASSERT_EQ(config.isValid(), true);
	BlurDetector::CNNBlurDetector detector{ config };
	ASSERT_EQ(detector.classify(), BlurDetector::ResultTypes[BlurDetector::BLURRED]);


}



TEST_F(CNNBlurDetectorTest, regularScenarioWithVisualizationBlurred)
{

	boost::python::dict dict;
	const char* imgPath = "../../files/blurred.jpg";
	dict["modelPath"] = "../../files/trainedModel.pt";
	dict["thresh"] = "0.5";
	dict["visualization"] = ".";

	BlurDetector::ModelConfig config{ imgPath, dict };
	ASSERT_EQ(config.isValid(), true);
	ASSERT_EQ(".", config.getVisualizationPath());
	ASSERT_EQ("blurred.jpg", config.getImageName());
	BlurDetector::CNNBlurDetector detector{ config };
	ASSERT_EQ(detector.classify(), BlurDetector::ResultTypes[BlurDetector::BLURRED]);

	detector.createImageWithCropsClassification(config.getVisualizationPath());
	ASSERT_EQ(std::filesystem::is_regular_file(config.getVisualizationPath() + "/" + config.getImageName()), true);
	
	std::filesystem::remove(config.getVisualizationPath() + "/" + config.getImageName());
	ASSERT_EQ(std::filesystem::is_regular_file(config.getVisualizationPath() + "/" + config.getImageName()), false);
}


TEST_F(CNNBlurDetectorTest, regularScenarioWithVisualizationSharp)
{

	boost::python::dict dict;
	const char* imgPath = "../../files/sharp.jpg";
	dict["modelPath"] = "../../files/trainedModel.pt";
	dict["thresh"] = "0.5";
	dict["visualization"] = ".";

	BlurDetector::ModelConfig config{ imgPath, dict };
	ASSERT_EQ(config.isValid(), true);
	ASSERT_EQ(".", config.getVisualizationPath());
	ASSERT_EQ("sharp.jpg", config.getImageName());
	BlurDetector::CNNBlurDetector detector{ config };
	ASSERT_EQ(detector.classify(), BlurDetector::ResultTypes[BlurDetector::SHARP]);

	detector.createImageWithCropsClassification(config.getVisualizationPath());
	ASSERT_EQ(std::filesystem::is_regular_file(config.getVisualizationPath() + "/" + config.getImageName()), true);

	std::filesystem::remove(config.getVisualizationPath() + "/" + config.getImageName());
	ASSERT_EQ(std::filesystem::is_regular_file(config.getVisualizationPath() + "/" + config.getImageName()), false);
}


TEST_F(CNNBlurDetectorTest, emptyVisualization)
{

	boost::python::dict dict;
	const char* imgPath = "../../files/sharp.jpg";
	dict["modelPath"] = "../../files/trainedModel.pt";
	dict["thresh"] = "0.5";
	dict["visualization"] = "";

	BlurDetector::ModelConfig config{ imgPath, dict };
	ASSERT_EQ(config.isValid(), true);
	ASSERT_EQ("", config.getVisualizationPath());
	ASSERT_EQ("sharp.jpg", config.getImageName());
	BlurDetector::CNNBlurDetector detector{ config };
	ASSERT_EQ(detector.classify(), BlurDetector::ResultTypes[BlurDetector::SHARP]);

	detector.createImageWithCropsClassification(config.getVisualizationPath());
	ASSERT_EQ(std::filesystem::is_regular_file(config.getImageName()), true);
	std::filesystem::remove(config.getImageName());
	ASSERT_EQ(std::filesystem::is_regular_file(config.getImageName()), false);

}

TEST_F(CNNBlurDetectorTest, invalidThreshold)
{

	boost::python::dict dict;
	const char* imgPath = "../../files/sharp.jpg";
	dict["modelPath"] = "../../files/trainedModel.pt";
	dict["thresh"] = "-23";

	BlurDetector::ModelConfig config{ imgPath, dict };
	ASSERT_EQ(config.isValid(), false);
	::testing::internal::CaptureStdout();
	BlurDetector::CNNBlurDetector detector{ config };
	ASSERT_EQ(detector.classify(), BlurDetector::ResultTypes[BlurDetector::PROCESSING_ERROR]);
	ASSERT_EQ("Provided invalid config\n", ::testing::internal::GetCapturedStdout());

}


TEST_F(CNNBlurDetectorTest, invalidModelPath)
{

	boost::python::dict dict;
	const char* imgPath = "../../files/sharp.jpg";
	dict["modelPath"] = "../../files/traindasfasedModel.pt";
	dict["thresh"] = "0.5";

	BlurDetector::ModelConfig config{ imgPath, dict };
	ASSERT_EQ(config.isValid(), true);
	::testing::internal::CaptureStdout();
	BlurDetector::CNNBlurDetector detector{ config };
	ASSERT_EQ(detector.classify(), BlurDetector::ResultTypes[BlurDetector::PROCESSING_ERROR]);
	::testing::internal::GetCapturedStdout();

}


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}