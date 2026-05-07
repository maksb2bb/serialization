#pragma once

#ifndef TECH_TASK_H
#define TECH_TASK_H
#endif 

#include <string>

struct ListNode {
	ListNode* prev = nullptr;
	ListNode* next = nullptr;
	ListNode* rand = nullptr;

	std::string data;

	ListNode() = default;

	ListNode(const ListNode&) = default;
	ListNode& operator=(const ListNode&) = delete;
};
