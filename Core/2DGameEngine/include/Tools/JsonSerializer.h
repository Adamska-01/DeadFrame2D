#pragma once
#include <nlohmann/json.hpp>
#include <type_traits>
#include <string>
#include <fstream>
#include <iostream>


namespace JsonSerializer 
{
	template <typename T>
	nlohmann::json Serialize(const T& obj) 
	{
		return nlohmann::json(obj);
	}

	template <typename T>
	T Deserialize(const nlohmann::json& json)
	{
		return json.get<T>();
	}

	template <typename T>
	void SerializeToFile(const T& obj, const std::string& filename) 
	{
		nlohmann::json jsonData = Serialize(obj);

		std::ofstream outFile(filename);

		if (!outFile.is_open()) 
			throw std::runtime_error("Failed to open file: " + filename);

		outFile << jsonData.dump(4); // Pretty print with 4-space indentation
		outFile.close();
	}

	template <typename T>
	T DeserializeFromFile(std::string_view filename)
	{
		auto filePathString = std::string(filename);

		std::ifstream inFile(filePathString);

		if (!inFile.is_open())
			throw std::runtime_error("Failed to open file: " + filePathString);

		std::string jsonContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

		inFile.close();
		
		nlohmann::json jsonData;

		try 
		{
			jsonData = nlohmann::json::parse(jsonContent, nullptr, true, true);
		}
		catch (const nlohmann::json::parse_error& e) 
		{
			std::cerr << "Error reading JSON: " << e.what() << std::endl;
			throw;
		}

		return Deserialize<T>(jsonData);
	}


	// Check if a type is serializable - SFINAE (Substitution Failure Is Not An Error) 
	template <typename T, typename = void>
	struct IsSerializable : std::false_type {};

	template <typename T>
	struct IsSerializable<T, std::void_t<decltype(nlohmann::json(std::declval<T>()))>> : std::true_type {};

	template <typename T>
	inline constexpr bool IsObjectSerializable = IsSerializable<T>::value;
} 
