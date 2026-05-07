#include <iostream>
#include "TechTask.h"
#include "ListSerializer.h"



int main()
{
	setlocale(LC_ALL, "rus");

	ListNode* head = ListSerializer::LoadFromFile("inlet.in");

	if (!head) {
		std::cerr << "Не удалось загрузить список из inlet.in" << std::endl;
		return 1;
	}

	ListSerializer::Serialize(head, "outlet.out");

	ListNode* restored = ListSerializer::Deserialize("outlet.out");
	if (!restored) {
		std::cerr << "Не удалось загрузить десериализацию из outlet.out\n";
		ListSerializer::FreeList(head);
		return 1;
	}

	ListSerializer::FreeList(head);
	ListSerializer::FreeList(restored);
}

