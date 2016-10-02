/**
 * @file   database.cc
 * @author Dennis Sitelew
 * @date   Sep 28, 2016
 */

#include "database.hh"
#include "db_settings.h"

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <sstream>
#include <iostream>
#include <chrono>

using ticker::database;

/**
 *  Initialize the database - check the DB verstion and update it if
 *  required
 */
void database::init() {
   int db_version(0);
   try {
      soci::session sql(soci::mysql, get_connection_string(false));
      sql.once << "USE " << db_settings::database;

   } catch (const soci::mysql_soci_error &e) {
      std::cout << "SOCI Exception: " << e.what() << std::endl;
      db_version = -1;
   }
   update_to_version(db_version, TARGET_DB_VERSION);
}

/**
 * Insert the sensor values into database
 *
 * @param co2              CO2 value
 * @param brightness       Brightness value
 * @param temp             Temperature value
 * @param humidity         Himidity value
 * @param led_brightness   LED Strip brightness value
 */
void database::add_sensor_entry(std::uint16_t co2, std::uint16_t brightness, double temp, double humidity, double led_brightness) {
   namespace sc = std::chrono;
   auto timestamp = sc::duration_cast<sc::seconds>(sc::system_clock::now().time_since_epoch()).count();

   soci::session sql(soci::mysql, get_connection_string(true));
   sql << "INSERT INTO readings(reading_time, co2, brightness, temperature, humidity, led_brightness)"
       << " VALUES(:rt, :co, :br, :t, :h, :lb)",
       soci::use(timestamp), soci::use(co2), soci::use(brightness), soci::use(temp), soci::use(humidity), soci::use(led_brightness);
}

/**
 * Get the database connection string
 *
 * @param include_db_name Include the database name inside the connection string.
 *
 * @return Connectuin strung
 */
std::string database::get_connection_string(bool include_db_name) {
   std::ostringstream settings;
   if (include_db_name) {
      settings << "dbname=" << db_settings::database;
   }
   settings << " user=" << db_settings::user_name;
   settings << " password=" << db_settings::password;
   return settings.str();
}

void database::update_to_version(int current_version, int target_version) {
   if (current_version == target_version) {
      std::cout << "Database is up to date: " << current_version << '\n';
      return;
   }
   std::cout << "Updating the database from " << current_version << " to " << target_version << std::endl;

   switch (current_version) {
      case -1: {
         soci::session sql(soci::mysql, get_connection_string(false));
         sql.once << "CREATE DATABASE " << db_settings::database;
         sql.once << "USE " << db_settings::database;
         sql.once << "CREATE TABLE metadata(version INT)";
         sql.once << "INSERT INTO metadata(version) VALUES(:version)", soci::use(0);
         sql.once << "CREATE TABLE readings(reading_time INT UNSIGNED, co2 INT UNSIGNED, brightness INT UNSIGNED, temperature FLOAT, humidity FLOAT, led_brightness FLOAT)";
      }
      case 0:
      case 1:
         break;
   }
}

