/*
 * Copyright 2018 Giuseppe Silano, University of Sannio in Benevento, Italy
 * Copyright 2018 Pasquale Oppido, University of Sannio in Benevento, Italy
 * Copyright 2018 Luigi Iannelli, University of Sannio in Benevento, Italy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BEBOP_CONTROL_POSITION_CONTROLLER_H
#define BEBOP_CONTROL_POSITION_CONTROLLER_H

#include <mav_msgs/conversions.h>
#include <mav_msgs/eigen_mav_msgs.h>

#include <ros/time.h>

#include "extendedKalmanFilter.h"
#include "stabilizer_types.h"
#include "parameters.h"
#include "common.h"



namespace teamsannio_med_control {

// Default values for the position controller of the Bebop. XYController [x,y], Roll Control [phi],
// Pitch Control [theta], Altitude Control [z], Yaw Control  [psi] 
static const Eigen::Vector2d kPDefaultXYController = Eigen::Vector2d(-1.3351, -1.1307);
static const double kPDefaultAltitudeController = -1.5994;

static const double kPDefaultPitchController = -2.7457;
static const double kPDefaultRollController = -2.2616;
static const double kPDefaultYawRateController = -1.8249;

static const Eigen::Vector2d MuDefaultXYController = Eigen::Vector2d(1, 1);
static const double MuDefaultAltitudeController = 0.12;

static const double MuDefaultPitchController = 0.26;
static const double MuDefaultYawRateController = 0.04;
static const double MuDefaultRollController = 0.09;


class PositionControllerParameters {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  PositionControllerParameters()
      : beta_xy_(kPDefaultXYController), 
        beta_z_(kPDefaultAltitudeController), 
        beta_phi_(kPDefaultRollController), 
        beta_theta_(kPDefaultPitchController),  
        beta_psi_(kPDefaultYawRateController),
        mu_xy_(MuDefaultXYController),
        mu_z_(MuDefaultAltitudeController),
        mu_theta_(MuDefaultPitchController),
        mu_phi_(MuDefaultRollController),
        mu_psi_(MuDefaultYawRateController){
  }

  Eigen::Vector2d beta_xy_;
  double beta_z_;

  double beta_phi_;
  double beta_theta_;
  double beta_psi_;

  Eigen::Vector2d mu_xy_;
  double mu_z_;

  double mu_phi_;
  double mu_theta_;
  double mu_psi_;
};
    
    class PositionController{
        public:
            PositionController();
            ~PositionController();
            void CalculateRotorVelocities(Eigen::Vector4d* rotor_velocities);

            void SetOdometry(const EigenOdometry& odometry);
            void SetTrajectoryPoint(const mav_msgs::EigenTrajectoryPoint& command_trajectory);
            
            PositionControllerParameters controller_parameters_;
            ExtendedKalmanFilter extended_kalman_filter_bebop_;
            VehicleParameters vehicle_parameters_;

            EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        private:
            bool controller_active_;
            bool first_step_position_controller;
            bool first_step_attitude_controller;

            double e_x_ = 0;
            double e_y_ = 0;
            double e_z_ = 0;
            double dot_e_x_ = 0;
            double dot_e_y_ = 0; 
            double dot_e_z_ = 0;
 
            double e_phi_ = 0;
            double e_theta_ = 0;
            double e_psi_ = 0;
            double dot_e_phi_ = 0;
            double dot_e_theta_ = 0; 
            double dot_e_psi_ = 0;

            double x_r_pre_ = 0;
            double y_r_pre_ = 0;
            double z_r_pre_ = 0;

            double phi_r_pre_ = 0;
            double theta_r_pre_ = 0;
            double psi_r_pre_ = 0;

            ros::Time prev_time_pos;
            ros::Duration delta_t_pos;

            ros::Time prev_time_att;
            ros::Duration delta_t_att;

            int64_t nanosecsPosition_pre_ = ros::Time::now().toNSec();
            int64_t nanosecsAttitude_pre_ = ros::Time::now().toNSec();

	    state_t state_;
            mav_msgs::EigenTrajectoryPoint command_trajectory_;
            EigenOdometry odometry_;

            void SetOdometryEstimated();
            void AttitudeController(double* u_phi, double* u_theta, double* u_psi);
            void AngularVelocityErrors(double* dot_e_phi_, double* dot_e_theta_, double* dot_e_psi_);
            void AttitudeErrors(double* e_phi_, double* e_theta_, double* e_psi_);
            void PosController(double* u_x, double* u_y, double* u_T, double* u_Terr);
            void PositionErrors(double* e_x, double* e_y, double* e_z);
            void VelocityErrors(double* dot_e_x, double* dot_e_y, double* dot_e_z);
            void ReferenceAngles(double* phi_r, double* theta_r);
            

    };

}
#endif // BEBOP_CONTROL_POSITION_CONTROLLER_H
