#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave() {
	// account Lock
	Account* account = AccountManager::Instance()->GetAccount(100);

	// user lock
	lock_guard<mutex> guard(_mutex);

	// �������� �ذ��
	// - lock�� ���ִ� ������ ���߸� �ذ� ����
	// - mutex�� ���� class�� ���� �� ��ü�� Id �ο� 
	//		=> lock�� �� ������ �����ؼ� id�� ū ������ lock�� �ɾ��ִ� ���� ��Ģ���

	// TO DO
}