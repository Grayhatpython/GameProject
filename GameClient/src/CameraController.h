#pragma once
#include "MonoBehaviour.h"

class CameraController : public MonoBehaviour
{
public:
	CameraController();
	virtual ~CameraController();

public:
	virtual void LateUpdate() override;

private:

};