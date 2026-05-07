#pragma once

#include <string>

struct ListNode;

class ListSerializer
{
public:
	static ListNode* LoadFromFile(const std::string& fileName);

	static void Serialize(ListNode* head, const std::string& fileName);

	static ListNode* Deserialize(const std::string& filename);

	//Очистка памяти списка
	static void FreeList(ListNode* head);
};
