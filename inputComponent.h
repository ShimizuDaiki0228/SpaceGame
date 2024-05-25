#pragma once
#include "moveComponent.h"


class InputComponent : public MoveComponent
{
public:
	InputComponent(class Actor* owner);

	void ProcessInput(const uint8_t* keyState) override;

	//�Q�b�^�[�ƃZ�b�^�[
	float GetMaxForward() { return _maxForwardSpeed; }
	float GetMaxAngular() { return _maxAngularSpeed; }
	int GetForwardKey() { return _forwardKey; }
	int GetBackKey() { return _backKey; }
	int GetClockwiseKey() { return _clockwiseKey; }
	int GetCounterClockwiseKey() { return _counterClockwiseKey; }

	void SetMaxForwardSpeed(float speed) { _maxForwardSpeed = speed; }
	void SetMaxAngularSpeed(float speed) { _maxAngularSpeed = speed; }
	void SetForwardKey(int key) { _forwardKey = key; }
	void SetBackKey(int key) { _backKey = key; }
	void SetClockwiseKey(int key) { _clockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { _counterClockwiseKey = key; }

private:
	//�O�i�A��]�̍ő呬�x
	float _maxForwardSpeed;
	float _maxAngularSpeed;
	//�O�i�A��ނ̂��߂̃L�[
	int _forwardKey;
	int _backKey;
	//��]�^���̂��߂̃L�[
	int _clockwiseKey;
	int _counterClockwiseKey;


};