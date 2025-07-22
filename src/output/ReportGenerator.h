#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "core/SimulationEngine.h"
#include <string>
#include <fstream>

class ReportGenerator {
public:
    // Generate console report
    static void printConsoleReport(const SimulationStats& stats);
    
    // Generate CSV report
    static bool generateCSVReport(const SimulationStats& stats, const std::string& filename);
    
    // Generate detailed task CSV
    static bool generateTaskCSV(const SimulationStats& stats, const std::string& filename);
    
    // Compare multiple simulation results
    static void compareResults(const std::vector<SimulationStats>& results);
    
private:
    static std::string formatDuration(int ticks);
    static std::string formatEnergy(double joules);
    static std::string formatPower(double watts);
    static std::string formatTemperature(double celsius);
};

#endif // REPORT_GENERATOR_H
