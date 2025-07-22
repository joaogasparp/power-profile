#ifndef THERMAL_MODEL_H
#define THERMAL_MODEL_H

class ThermalModel {
public:
    ThermalModel(double ambient_temp = 25.0, double heating_coeff = 0.1, double cooling_coeff = 0.05);
    
    // Update temperature based on current power consumption
    void updateTemperature(double power_watts);
    
    // Get current temperature
    double getCurrentTemperature() const { return current_temp_; }
    
    // Get peak temperature recorded
    double getPeakTemperature() const { return peak_temp_; }
    
    // Reset thermal state
    void reset();
    
    // Thermal throttling check
    bool shouldThrottle(double threshold_temp = 85.0) const;
    
    // Set thermal parameters
    void setParameters(double ambient_temp, double heating_coeff, double cooling_coeff);
    
private:
    double current_temp_;      // Current temperature (°C)
    double peak_temp_;         // Peak temperature reached (°C)
    double ambient_temp_;      // Ambient temperature (°C)
    double heating_coeff_;     // Heating coefficient
    double cooling_coeff_;     // Cooling coefficient
};

#endif // THERMAL_MODEL_H
