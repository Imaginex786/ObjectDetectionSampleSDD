#include "Utility.hpp"

std::vector<std::string> Utility::splitString(std::string strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
        splittedStrings.push_back(item);
    }
    assert(!(splittedStrings.begin() == splittedStrings.end()));
    return splittedStrings;
}

float Utility::normalizeZeroToOne(float value, float normalizer)
{
    if (value == 0){
        value = value + 1.0;
    }
    float res = value/normalizer;
    return res;
}

long Utility::getSystemTime()
{
    // this return the time since epoch in milliseconds in machine timezone.
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

/** TODO: Change implementation of this method
* A Method ISO8061 -> UNIX timestamp in milliseconds in UTC timezone
*/
long Utility::iso8061_to_unix_ms(const std::string& timestamp, const std::string& vid_timezone)
{   
    // Create a timezone info DB for easy access
    boost::local_time::tz_database tz_db;
    tz_db.load_from_file(static_cast<std::string>(getenv("BASE_DIR"))+ "/src/Utils/db_timezone.csv");

    // Reference datetime object to compute epoch since
    boost::posix_time::ptime time_t_epoch(boost::gregorian::date(1970,1,1));

    // Set timezone of input datetime string (EDT = UTC-4)
    boost::local_time::time_zone_ptr cam_tz = tz_db.time_zone_from_region(vid_timezone); 

    // Convert ISO 8061 timestamp into boost::ptime format
    boost::posix_time::ptime cam_datetime(boost::posix_time::from_iso_string(timestamp));

    // Convert boost::ptime into Timezone aware datetime object
    boost::local_time::local_date_time tz_aware_cam_dt( cam_datetime.date(), cam_datetime.time_of_day() , cam_tz, false ); 
    boost::posix_time::time_duration diff = tz_aware_cam_dt.utc_time() - time_t_epoch;
    return 1000L * (static_cast<long>(diff.total_seconds()));
}

bool Utility::file_exists(const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

bool Utility::writeFrame(Frame& frame, const std::string& img_name)
{
    std::string comp_path = "/app/test_frames/" + img_name + ".jpg";
    return cv::imwrite(comp_path, frame.getFrame());
}

std::string Utility::getDebugDir()
{
    return static_cast<std::string>(getenv("BASE_DIR")) + "/debug";
}

bool Utility::isNthItem(int item_count, int n)
{
    return ((n > 0) && (item_count%n == 0));
}

const std::string Utility::getBasePath()
{
    return static_cast<std::string>(getenv("BASE_DIR"));
}

/**
* @TODO @team error handling
* Generates a map from a string.
*
* @details The string needs to be separate each row with "|" and
* each item with ",". The first row is considered to be the keys
* of the map, and rest of rows are considered values. Number of
* items in each row is expected to be the same.
*
* @param map_string
* @return list of maps
*/
config_map_list Utility::generate_map(std::string map_string)
{
    std::vector<std::string> rows = Utility::splitString(map_string, '|');
    std::string key_row = rows[0];
    std::vector<std::string> key_list = Utility::splitString(key_row, ',');

    std::vector<config_map> config_maps;

    for (unsigned int l = 1; l < rows.size(); ++l) {
        std::string row = rows[l];
        std::vector<std::string> item_list = Utility::splitString(row, ',');
        config_map a_map;
        for (unsigned int i = 0; i < item_list.size(); ++i) {
            a_map[key_list[i]] = item_list[i];
        }
        config_maps.push_back(a_map);
    }
    return config_maps;
}

/**
* @TODO @Hasnain error handling
* Generates a map from a string.
*
* @details The string needs to be separate each row with "|" and
* each item with ",". The first row is considered to be the keys
* of the map, and rest of rows are considered values. Number of
* items in each row is expected to be the same.
*
* @param map_string
* @return list of maps
*/

std::unordered_map<int, std::string> Utility::generate_ClassMap(std::string map_string)
{
    std::vector<std::string> rows = Utility::splitString(map_string, '|');
    std::string key_row = rows[0];
    std::vector<std::string> key_list = Utility::splitString(key_row, ',');
    std::string value_row = rows[1];
    std::vector<std::string> value_list = Utility::splitString(value_row, ',');
    std::unordered_map<int, std::string> class_map;
    if(key_list.size()!=value_list.size() || rows.size()!=2) {
        SPDLOG_INFO("[Utility][generate_ClassMap] given {} class map is not valid.", map_string);
        throw std::invalid_argument("Given Class_Map is invalid."); 
    }
    for (unsigned int l = 0; l < key_list.size(); ++l) {
        class_map[std::stoi(key_list[l])] = value_list[l];
    }
    return class_map;
}

std::vector<Detections> Utility::readDetectionsFromFile(const std::string& file_path, std::vector<std::string> class_info)
{
    std::vector<Detections> all_detections;
    std::ifstream file(file_path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> vals = Utility::splitString(line, ' ');
            Detections single_det;
            LabelData label_info;
            single_det.bbox.x = std::stof(vals[0]);
            single_det.bbox.y = std::stof(vals[1]);
            single_det.bbox.w = std::stof(vals[2]) - std::stof(vals[0]);
            single_det.bbox.h = std::stof(vals[3]) - std::stof(vals[1]);
            single_det.bbox.objectness_score = std::stof(vals[4]);
            label_info.class_score = std::stof(vals[5]);
            label_info.class_name = class_info[std::stoi(vals[6])];
            single_det.labels_data["vehicle_type"] = label_info;
            all_detections.push_back(single_det);
            
        }
        file.close();
} 
    return all_detections;
}

int Utility::matchDetection(Detections& single_det, std::vector<Detections>& gt_dets)
{
    int match_index = -1;
    for(unsigned int i=0; i<gt_dets.size(); i++) {
        if(gt_dets[i].bbox.x == single_det.bbox.x &&
           gt_dets[i].bbox.y == single_det.bbox.y &&
           gt_dets[i].bbox.w == single_det.bbox.w &&
           gt_dets[i].bbox.h == single_det.bbox.h )
        match_index = i;
    }
    return match_index;
}