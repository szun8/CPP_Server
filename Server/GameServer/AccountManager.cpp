#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"


void AccountManager::ProcessLogin() {
	// �� �Ŵ����� �ΰ� �̻��� lock�� ���� ��찡 �ټ� �߻�����

	// account Lock
	lock_guard<mutex> guard(_mutex);

	// User Lock (������)
	User* user = UserManager::Instance()->GetUser(100);
}