#pragma once

#include "Audio.h"
#include "Sphere.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	ViewProjection viewProjection_;

	std::unique_ptr<Model> sphere_;

	std::shared_ptr<Camera> camera;

	// ワールドにあるすべてのオブジェクトを格納します
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<Sphere>> circles;

	// 選択されたオブジェクトのポインタがここに格納される
	std::shared_ptr<Object> selectedObject = nullptr;

	bool lookAtObject = true;

	Vec3 camVel_;

	float cameraMoveSpeed_ = 1.0f;
};
