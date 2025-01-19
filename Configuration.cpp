#include "Configuration.h"
#include "Misc/json.hpp"
#include <fstream>

using namespace nlohmann;

bool Configuration::loadFromFile(const std::string& config_path)
{
	json js_config;

	std::ifstream in(config_path);
	if (!in.is_open())
		return false;
	in >> js_config;

	m_recent_connections = js_config["connectors"];

	return true;
}

bool Configuration::serializeToFile(const std::string& config_path)
{
	// Serializes the first 10th connectors only
	if (m_recent_connections.size() > 10)
	{
		m_recent_connections.erase(std::next(m_recent_connections.begin(),10), m_recent_connections.end());
	}

	json js_config;
	js_config["connectors"] = m_recent_connections;

	std::ofstream out(config_path);
	if (!out.is_open())
		return false;
	out << std::setw(4) << js_config << std::endl;
	return true;
}
