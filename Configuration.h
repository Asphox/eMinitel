#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <set>

class Configuration
{
public:

	bool loadFromFile(const std::string& config_path);
	bool serializeToFile(const std::string& config_path);
	std::set<std::string> m_recent_connections;
};

#endif //CONFIGURATION

