#ifndef CORE_FUTILS_H
#define CORE_FUTILS_H

#include <any>
#include <vector>
#include <assert.h>

#include "opencv2/opencv.hpp"

#include "StreamHandler/Frame.hpp"


/** utility functions for frame class */
namespace futils
{
    /**
     *
     * @param frame (*BGR frame)
     * @param mean_rgb
     * @param std_rgb
     * @return
     */
    inline std::vector<float> flatten(Frame& frame)
    {
        cv::Mat frame_image = frame.getFrame();
        frame_image.convertTo(frame_image, CV_32FC3);

        int image_depth = 3;
        cv::Mat planes[3];        
        
        /** Split into BGR */
        cv::split(frame_image, planes);

        std::vector<float> flattened_image;
        flattened_image.assign(planes[0].begin<float>(), planes[0].end<float>());

        for (int i = 1; i < image_depth; ++i) {
            flattened_image.insert(flattened_image.end(), planes[i].begin<float>(), planes[i].end<float>());
        }
        frame_image.release();
        planes[0].release();
        planes[1].release();
        planes[2].release();
        return flattened_image;
    }
    inline std::vector<float> flatten_multiple(std::vector<Frame> frames)
    {
        std::vector<float> flattened_data;
        for (Frame a_frame: frames) {
            std::vector<float> flattened_img = futils::flatten(a_frame);
            flattened_data.insert(flattened_data.end(), flattened_img.begin(), flattened_img.end());
        }
        return flattened_data;
    }

    inline void convertFrameBaseOnChannels(cv::Mat& frame, const int channels) 
    {
        if( channels == 3) {
            frame.convertTo(frame, CV_32FC3);
        } else if (channels == 2) {
            frame.convertTo(frame, CV_32FC2);
        } else if (channels == 1) {
            frame.convertTo(frame, CV_32FC1);
        } else {
            throw std::invalid_argument("Image is not in valid format."); 
        }
    }

    // template<typename T>
    // inline std::vector<T> flatten_temp(Frame& frame)
    // {
    //     cv::Mat frame_image = frame.getFrame();
    //     assert(!frame_image.empty());
    //     const int image_depth = frame.getChannels();
    //     futils::convertFrameBaseOnChannels(frame_image, image_depth);
    //     cv::Mat *planes =  new cv::Mat[image_depth];        
        
    //     // /** Split into BGR */
    //     cv::split(frame_image, planes);
    //     std::vector<T> flattened_image;
    //     // flattened_image.assign(planes[0].begin<float>(), planes[0].end<float>());
    //     for (int i = 0; i < image_depth; i++) {
    //         cv::Mat single_plane = planes[i];
    //         for(signed int j = 0; j < single_plane.rows; j++) {
    //             for(signed int k = 0; k < single_plane.cols; k++) {
    //                 flattened_image.push_back(single_plane.at<T>(j, k));
    //             }
    //         }
    //     }    
    //     return flattened_image;
    // }

    template<typename T>
    inline std::vector<T> flatten_temp(Frame& frame)
    {
        cv::Mat frame_image = frame.getFrame();
        assert(!frame_image.empty());
        const int image_depth = frame.getChannels();
        futils::convertFrameBaseOnChannels(frame_image, image_depth);
        cv::Mat *planes =  new cv::Mat[image_depth];        
        
        // /** Split into BGR */
        cv::split(frame_image, planes);
        std::vector<T> flattened_image;
        
        flattened_image.assign(planes[0].begin<T>(), planes[0].end<T>());

        for (int i = 1; i < image_depth; ++i) {
            flattened_image.insert(flattened_image.end(), planes[i].begin<T>(), planes[i].end<T>());
        }
        // std::vector<T> vec;
        // vec.reserve( flattened_image.size() );    //  avoids unnecessary reallocations
        // std::transform(flattened_image.begin(), flattened_image.end(),
        //         std::back_inserter( vec ),
        //         [](const float &arg) { return static_cast<T>(arg); });
        frame_image.release();
        delete [] planes;
        return flattened_image;
    }

    template<typename T>
    inline std::vector<T> flatten_multiple_temp(std::vector<Frame> frames)
    {
        std::vector<T> flattened_data;
        for (Frame a_frame: frames) {
            std::vector<T> flattened_img = futils::flatten_temp<T>(a_frame);
            flattened_data.insert(flattened_data.end(), flattened_img.begin(), flattened_img.end());
        }
        return flattened_data;
    }

    inline void flatten_img(Frame& frame, std::any& data, const std::string& datatype)
    {
        if (datatype == "float") {
            data = futils::flatten_temp<float>(frame);
        }
        else if (datatype == "int") {
            data = futils::flatten_temp<int>(frame);
        }
        else if (datatype == "double") {
            data = futils::flatten_temp<double>(frame);
        }
        else if (datatype == "uint8_t") {
            data = futils::flatten_temp<uint8_t>(frame);
        }
        else {
            SPDLOG_ERROR("invalid argument: datatype = {}", datatype);
            throw std::invalid_argument(
            "flatten_img received invalid argument for datatype");
        } 
    }

    inline void flatten_multiple_img(std::vector<Frame> frames, std::any& data_vec, const std::string& datatype)
    {
        if (datatype == "float") {
            data_vec = futils::flatten_multiple_temp<float>(frames);
        }
        else if (datatype == "int") {
            data_vec = futils::flatten_multiple_temp<int>(frames);
        }
        else if (datatype == "double") {
            data_vec = futils::flatten_multiple_temp<double>(frames);
        }
        else if (datatype == "uint8_t") {
            data_vec = futils::flatten_multiple_temp<uint8_t>(frames);
        }
        else {
            SPDLOG_ERROR("invalid argument: datatype = {}", datatype);
            throw std::invalid_argument(
            "flatten_multiple_img received invalid argument for datatype");
        } 
    }
    
    // inline void castVector(std::string datatype, Frame frame, void * &data)
    // {
    //     if (datatype=="float") {

    //         std::vector<float> vec = flatten_temp<float>(frame);
    //         data = &vec;
    //     }
    // }

    inline bool save(const std::string& image_path, const Frame& frame_to_save)
    {
        return cv::imwrite(image_path, frame_to_save.getFrame());
    }


    /** @TODO(Bazarovay) New Frame is being created, clearing all metadata. Take as input. This is error-prone */
    inline void resize(const Frame& input, Frame& output, int width, int height)
    {
        cv::Mat resized;
        cv::resize(input.getFrame(), resized, cv::Size(width, height));
        output.setFrame(resized);
        resized.release();
    }

    inline void normalize(const Frame& input, Frame& output, int r_type, double scale_factor)
    {
        cv::Mat norm_image;
        input.getFrame().convertTo(norm_image, r_type, scale_factor);
        output.setFrame(norm_image);
        norm_image.release();
    }

    inline void convertColourSpace(const Frame& input, Frame& output, int code)
    {
        cv::Mat converted_image;
        cv::cvtColor(input.getFrame(), converted_image, code);
        output.setFrame(converted_image);
        output.copyFrameImage(input.getFrame());
        converted_image.release();
    }

    inline void convertRGB2BGR(const Frame& input, Frame& output)
    {
        convertColourSpace(input, output, cv::COLOR_RGB2BGR);
    }

    inline void convertBGR2RGB(const Frame& input, Frame& output)
    {
        convertColourSpace(input, output, cv::COLOR_BGR2RGB);
    }

    /**
     * @brief Frame (image) conversion function 
     * This method performs following conversions:
     * 1. BGR -> RGB
     * 2. Normalizes image by subtracting 
     *    mean and dividing by standard dev.
     *
     * @param input_bgr
     * @param out_norm_rgb
     * @param mean_rgb
     * @param std_rgb
     */
    inline void convertBGR2RGBwithMeanAndStdDev(const Frame& input_bgr,
                                                Frame& out_norm_rgb,
                                                std::vector<float> mean_rgb,
                                                std::vector<float> std_rgb)
    {

        cv::Mat converted_rgb;
        cv::cvtColor(input_bgr.getFrame(), converted_rgb, cv::COLOR_BGR2RGB);

        int image_depth = 3;
        cv::Mat planes[3];

        // cv::Mat planes[image_depth];
        /** Split into RGB */
        cv::split(converted_rgb, planes);
        /** Subtract mean_rgb and divide by std_rgb */
        for (int i = 0; i < image_depth; ++i) {
            planes[i] = planes[i] - mean_rgb.at(i);
            planes[i] = planes[i] / std_rgb.at(i);
            planes[i] = planes[i].reshape(1, 1);
        }
        cv::Mat merged_image;
        cv::merge(planes, image_depth, merged_image);
        merged_image = merged_image.reshape(3, converted_rgb.rows);
        out_norm_rgb.setFrame(merged_image);
        converted_rgb.release();
    }
/**
     * @brief Frame (image) conversion function 
     * This method performs following conversions:
     * 1. Normalizes image by subtracting 
     *    mean and dividing by standard dev.
     *
     * @param input_bgr
     * @param out_norm_bgr
     * @param mean_bgr
     * @param std_bgr
     */
    inline void ApplyMeanAndStdDevBGR(const Frame& input_bgr,
                                                Frame& out_norm_bgr,
                                                std::vector<float> mean_bgr,
                                                std::vector<float> std_bgr)
    {

        int image_depth = 3;
        cv::Mat planes[3];

        // cv::Mat planes[image_depth];
        /** Split into BGR */
        cv::split(input_bgr.getFrame(), planes);
        /** Subtract mean_rgb and divide by std_rgb */
        for (int i = 0; i < image_depth; ++i) {
            planes[i] = planes[i] - mean_bgr.at(i);
            planes[i] = planes[i] / std_bgr.at(i);
            planes[i] = planes[i].reshape(1, 1);
        }
        cv::Mat merged_image;
        cv::merge(planes, image_depth, merged_image);
        merged_image = merged_image.reshape(3, input_bgr.getFrame().rows);
        out_norm_bgr.setFrame(merged_image);
        planes[0].release();
        planes[1].release();
        planes[2].release();
        merged_image.release();
    }
}
#endif //CORE_FUTILS_H
