//
// Created by Liam Kelly on 3/28/19.
//

#ifndef PROJECT_RF_GPS_H
#define PROJECT_RF_GPS_H


class rf_gps_entry : public log_entry {

public:

    rf_gps_entry(int x, int y);
    ~rf_gps_entry();

    std::string get_name();

private:

    static const std::string ENTRY_ID;

};


#endif //PROJECT_RF_GPS_H
