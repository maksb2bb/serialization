#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <cstdint>
#include "ListSerializer.h"
#include "TechTask.h"

ListNode* ListSerializer::LoadFromFile(const std::string& fileName) 
{
	std::ifstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "Не удалось открыть файл: " << fileName << std::endl;
		return nullptr;
	}

	std::vector<ListNode*> nodes;
	std::vector<int> randIndices;
	std::string line;

	while (std::getline(file, line)) {
		if (line.empty()) continue;

		size_t pos = line.find_last_of(';');
		if (pos == std::string::npos) {
			std::cerr << "Ошибка формата строки: " << line << std::endl;
			continue;
		}

		std::string data = line.substr(0, pos);
		std::string randStr = line.substr(pos + 1);

		ListNode* node = new ListNode();
		node->data = std::move(data);
		node->prev = nullptr;
		node->next = nullptr;
		node->rand = nullptr;

		nodes.push_back(node);
		randIndices.push_back(std::stoi(randStr));
	}

	if (nodes.empty())
	{
		return nullptr;
	}

	for (size_t i = 0; i < nodes.size(); ++i) {
		if (i > 0) {
			nodes[i]->prev = nodes[i - 1];
		}
		if (i + 1 < nodes.size()) {
			nodes[i]->next = nodes[i + 1];
		}
	}

	for (size_t i = 0; i < nodes.size(); ++i) {
		int idx = randIndices[i];
		if (idx >= 0 && idx < static_cast<int>(nodes.size())) {
			nodes[i]->rand = nodes[idx];
		}
		else {
			nodes[i]->rand = nullptr;
		}
	}

	std::cout << "Список успешно загружен. Узлов: " << nodes.size() << std::endl;

	return nodes.empty() ? nullptr : nodes[0];
}

void ListSerializer::Serialize(ListNode* head, const std::string& fileName)
{
	if (!head) {
		std::cerr << "Serialize: head is nullptr" << std::endl;
		return;
	}

	std::ofstream file(fileName, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Не удалось открыть файл для записи: " << fileName << std::endl;
		return;
	}

	std::unordered_map<ListNode*, int> nodeToIndex;
	int index = 0;
	ListNode* current = head;

	while (current) {
		nodeToIndex[current] = index++;
		current = current->next;
	}

	uint32_t nodeCount = static_cast<uint32_t>(index);

	file.write(reinterpret_cast<const char*>(&nodeCount), sizeof(nodeCount));

	current = head;
	while (current)
	{
		uint32_t dataLength = static_cast<uint32_t>(current->data.size());
		file.write(reinterpret_cast<const char*>(&dataLength), sizeof(dataLength));

		if (dataLength > 0) {
			file.write(current->data.data(), dataLength);
		}

		int randIndex = -1;

		if (current->rand)
		{
			auto it = nodeToIndex.find(current->rand);
			if (it != nodeToIndex.end()) 
			{
				randIndex = it->second;
			}
		}

		file.write(reinterpret_cast<const char*>(&randIndex), sizeof(randIndex));

		current = current->next;
	}

	std::cout << "Сериализация завершена. Узлов записано: " << nodeCount << std::endl;
}

ListNode* ListSerializer::Deserialize(const std::string& fileName)
{
	std::ifstream file(fileName, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Не удалось открыть файл: " << fileName << std::endl;
		return nullptr;
	}

	uint32_t nodeCount = 0;

	file.read(reinterpret_cast<char*>(&nodeCount), sizeof(nodeCount));

	if (nodeCount == 0) {
		return nullptr;
	}

	std::vector<ListNode*> nodes(nodeCount, nullptr);
	std::vector<int> randIndices(nodeCount, -1);

	for (uint32_t i = 0; i < nodeCount; ++i)
	{
		uint32_t dataLength = 0;
		file.read(reinterpret_cast<char*>(&dataLength), sizeof(dataLength));

		nodes[i] = new ListNode();
		nodes[i]->prev = nullptr;
		nodes[i]->next = nullptr;
		nodes[i]->rand = nullptr;

		if (dataLength > 0) {
			nodes[i]->data.resize(dataLength);
			file.read(&nodes[i]->data[0], dataLength);
		}

		int randIdx = -1;
		file.read(reinterpret_cast<char*>(&randIdx), sizeof(randIdx));
		randIndices[i] = randIdx;
	}

	for (uint32_t i = 0; i < nodeCount; ++i) {
		if (i > 0) {
			nodes[i]->prev = nodes[i - 1];
		}
		if (i + 1 < nodeCount) {
			nodes[i]->next = nodes[i + 1];
		}
	}

	for (uint32_t i = 0; i < nodeCount; ++i) {
		int idx = randIndices[i];
		if (idx >= 0 && idx < static_cast<int>(nodeCount)) {
			nodes[i]->rand = nodes[idx];
		}
	}

	std::cout << "Десериализация завершена. Восстановлено узлов: " << nodeCount << std::endl;

	return nodes[0];
}

void ListSerializer::FreeList(ListNode* head)
{
	if (!head) {
		return;
	}

	ListNode* current = head;
	ListNode* nextNode = nullptr;

	while (current) {
		nextNode = current->next;

		current->prev = nullptr;
		current->next = nullptr;
		current->rand = nullptr;

		delete current;
		current = nextNode;
	}
}