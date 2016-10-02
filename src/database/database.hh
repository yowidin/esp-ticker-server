/**
 * @file   database.hh
 * @author Dennis Sitelew
 * @date   Sep 28, 2016
 */

#ifndef SRC_DATABASE_DATABASE_HH
#define SRC_DATABASE_DATABASE_HH

#include <cstdint>
#include <memory>
#include <string>

namespace ticker {

class database {
public:
   //! Target database version
   static const int TARGET_DB_VERSION = 1;

public:
   void init();

   void add_sensor_entry(
      std::uint16_t co2, std::uint16_t brightness,
      double temp, double humidity, double led_brightness);

private:
   std::string get_connection_string(bool include_db_name);

   void update_to_version(int current_version, int target_version);

};

} // namespace ticker

#endif // SRC_DATABASE_DATABASE_HH
