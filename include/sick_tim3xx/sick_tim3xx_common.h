/*
 * Copyright (C) 2013, Osnabrück University
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Osnabrück University nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *  Created on: 24.05.2012
 *
 *      Authors:
 *         Jochen Sprickerhof <jochen@sprickerhof.de>
 *         Martin Günther <mguenthe@uos.de>
 *
 * Based on the TiM communication example by SICK AG.
 *
 */

#ifndef SICK_TIM3XX_COMMON_H_
#define SICK_TIM3XX_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/String.h>

#include <dynamic_reconfigure/server.h>
#include <sick_tim3xx/SickTim3xxConfig.h>
#include <sick_tim3xx/abstract_parser.h>

namespace sick_tim3xx
{

class SickTim3xxCommon
{
public:
  SickTim3xxCommon(AbstractParser* parser);
  virtual ~SickTim3xxCommon();
  int init_usb();
  int loopOnce();
  void check_angle_range(SickTim3xxConfig &conf);
  void update_config(sick_tim3xx::SickTim3xxConfig &new_config, uint32_t level = 0);

private:
  static const unsigned int USB_TIMEOUT = 500; // milliseconds

  ssize_t getSOPASDeviceList(libusb_context *ctx, uint16_t vendorID, uint16_t productID, libusb_device ***list);
  void freeSOPASDeviceList(libusb_device **list);

  void printUSBDeviceDetails(struct libusb_device_descriptor desc);
  void printUSBInterfaceDetails(libusb_device* device);
  void printSOPASDeviceInformation(ssize_t numberOfDevices, libusb_device** devices);

  int sendSOPASCommand(libusb_device_handle* device_handle, const char* request, unsigned int timeout);

  // ROS
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  ros::Publisher datagram_pub_;
  bool publish_datagram_;

  // Dynamic Reconfigure
  SickTim3xxConfig config_;
  dynamic_reconfigure::Server<sick_tim3xx::SickTim3xxConfig> dynamic_reconfigure_server_;

  // libusb
  libusb_context *ctx_;
  ssize_t numberOfDevices_;
  libusb_device **devices_;
  libusb_device_handle *device_handle_;

  // Parser
  AbstractParser* parser_;
};

} /* namespace sick_tim3xx */
#endif /* SICK_TIM3XX_COMMON_H_ */
