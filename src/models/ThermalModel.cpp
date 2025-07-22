#include "ThermalModel.h"
#include <algorithm>

ThermalModel::ThermalModel(double ambient_temp, double heating_coeff, double cooling_coeff)
    : current_temp_(ambient_temp), peak_temp_(ambient_temp), ambient_temp_(ambient_temp),
      heating_coeff_(heating_coeff), cooling_coeff_(cooling_coeff) {
}

void ThermalModel::updateTemperature(double power_watts) {
    // Simplified thermal model: Temp[t] = Temp[t-1] + k*Power - d*(Temp[t-1] - Ambient)
    double heating = heating_coeff_ * power_watts;
    double cooling = cooling_coeff_ * (current_temp_ - ambient_temp_);
    
    current_temp_ = current_temp_ + heating - cooling;
    
    // Ensure temperature doesn't go below ambient
    current_temp_ = std::max(current_temp_, ambient_temp_);
    
    // Update peak temperature
    peak_temp_ = std::max(peak_temp_, current_temp_);
}

void ThermalModel::reset() {
    current_temp_ = ambient_temp_;
    peak_temp_ = ambient_temp_;
}

bool ThermalModel::shouldThrottle(double threshold_temp) const {
    return current_temp_ > threshold_temp;
}

void ThermalModel::setParameters(double ambient_temp, double heating_coeff, double cooling_coeff) {
    ambient_temp_ = ambient_temp;
    heating_coeff_ = heating_coeff;
    cooling_coeff_ = cooling_coeff;
}
