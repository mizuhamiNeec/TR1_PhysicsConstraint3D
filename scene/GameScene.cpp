#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <DebugText.h>

#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

void DrawGrid();
void RenderOutliner(const std::shared_ptr<Object>& object, std::shared_ptr<Object>& selectedObject);

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	sphere_.reset(Model::CreateSphere(16, 16));

	AxisIndicator::GetInstance()->SetVisible(true);                          // 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_); // 軸方向が参照するビュープロジェクションを指定する(アドレス渡し)

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	//-----------------------------------------------------------------------------
	// 
	//-----------------------------------------------------------------------------
	constexpr int numChildren = 5;
	auto circleRoot = std::make_shared<Circle>("CircleRoot");
	circleRoot->SetTransform(
		Vec3::zero,
		Vec3::zero,
		Vec3::one
	);
	circleRoot->SetModel(sphere_.get());
	circles.push_back(circleRoot);
	objects.push_back(circleRoot);

	auto parent = circleRoot;
	for (int i = 1; i <= numChildren; ++i) {
		auto child = std::make_shared<Circle>("child" + std::to_string(i));
		child->SetTransform(
			{child->GetRadius() * 2.0f * i, 0.0f, 0.0f},
			Vec3::zero,
			Vec3::one
		);
		parent->AddChild(child);
		child->Initialize(child->GetName());
		child->SetModel(sphere_.get());
		circles.push_back(child);
		parent = child;
	}

	//auto child2 = std::make_shared<Circle>("CircleChild2");
	//child2->SetTransform({-5.0f,0.0f,0.0f}, Vec3::zero, Vec3::one);
	//circleRoot->GetChildren()[0]->AddChild(child2);
	//child2->Initialize(child2->GetName());
	//child2->SetModel(sphere_.get());
	//circles.push_back(child2);

	auto otherCircle = std::make_shared<Circle>("OtherCircle");
	otherCircle->SetModel(sphere_.get());
	otherCircle->SetTransform(
		{-4.0f,0.0f,0.0f},
		Vec3::zero,
		Vec3::one
	);
	circles.push_back(otherCircle);
	objects.push_back(otherCircle);

	// カメラを作成
	camera = std::make_shared<Camera>();
	camera->Initialize("Camera");
	camera->SetViewProjection(&viewProjection_);

	objects.push_back(camera);
}

Vector3 TransformNormal(Vector3 v, const Mat4& m);

void GameScene::Update() {
#pragma region カメラ操作
	if (Input::GetInstance()->IsPressMouse(1)) {
		Input* input = Input::GetInstance();
		Input::MouseMove mouseMove = input->GetMouseMove();
		Vec3 camRot = camera->GetTransform().rotation;

		// Vec2にする
		Vec2 mouseDelta = {static_cast<float>(mouseMove.lX),static_cast<float>(mouseMove.lY)};

		mouseDelta *= 0.1f * 0.022f; // Quake m_yaw m_pitch

		camRot.x += mouseDelta.y;
		camRot.y += mouseDelta.x;

		// x回転を制限
		camRot.x = std::clamp(camRot.x, -89.9f * deg2Rad, 89.9f * deg2Rad);

		Vec3 pos = camera->GetTransform().position;

		Vec3 moveInput;
		if (input->PushKey(DIK_W) && !input->PushKey(DIK_S)) {
			moveInput.z = 1.0f;
		} else if (!input->PushKey(DIK_W) && input->PushKey(DIK_S)) {
			moveInput.z = -1.0f;
		}

		if (input->PushKey(DIK_D) && !input->PushKey(DIK_A)) {
			moveInput.x = 1.0f;
		} else if (!input->PushKey(DIK_D) && input->PushKey(DIK_A)) {
			moveInput.x = -1.0f;
		}

		if (input->PushKey(DIK_E) && !input->PushKey(DIK_Q)) {
			moveInput.y = 1.0f;
		} else if (!input->PushKey(DIK_E) && input->PushKey(DIK_Q)) {
			moveInput.y = -1.0f;
		}

		moveInput.Normalize();

		Mat4 cameraRotationMatrix = Mat4::Affine(Vec3::one, camera->GetTransform().rotation, Vec3::zero);

		Vec3 cameraForward = {
			cameraRotationMatrix.m[2][0],
			cameraRotationMatrix.m[2][1],
			cameraRotationMatrix.m[2][2]
		};

		Vec3 cameraRight = {
			cameraRotationMatrix.m[0][0],
			cameraRotationMatrix.m[0][1],
			cameraRotationMatrix.m[0][2]
		};

		/*Vec3 cameraUp = {
			cameraRotationMatrix.m[1][0],
			cameraRotationMatrix.m[1][1],
			cameraRotationMatrix.m[1][2]
		};*/

		// ワールドにしておく
		Vec3 cameraUp = {
			0.0f,
			1.0f,
			0.0f
		};

		Vec3 wishVel = (cameraForward * moveInput.z + cameraRight * moveInput.x + cameraUp * moveInput.y) * cameraMoveSpeed_;

		camVel_.x = std::lerp(camVel_.x, wishVel.x, 6.0f * deltaTime);
		camVel_.y = std::lerp(camVel_.y, wishVel.y, 6.0f * deltaTime);
		camVel_.z = std::lerp(camVel_.z, wishVel.z, 6.0f * deltaTime);

		pos += camVel_;

		camera->SetTransform(
			pos,
			camRot,
			camera->GetTransform().scale
		);

		ImGui::Begin("Input");
		ImGui::Text("%.2f %.2f", mouseDelta.x, mouseDelta.y);

		if (input->PushKey(DIK_W)) {
			ImGui::Text("W");
		}
		if (input->PushKey(DIK_A)) {
			ImGui::Text("A");
		}
		if (input->PushKey(DIK_S)) {
			ImGui::Text("S");
		}
		if (input->PushKey(DIK_D)) {
			ImGui::Text("D");
		}

		if (input->PushKey(DIK_E)) {
			ImGui::Text("E");
		}
		if (input->PushKey(DIK_Q)) {
			ImGui::Text("Q");
		}

		ImGui::End();
	}
#pragma endregion

	// 円の更新
	for (size_t i = 0; i < circles.size(); ++i) {
		for (size_t j = i + 1; j < circles.size(); ++j) {
			if (circles[i]->IsCollide(*circles[j])) {
				circles[i]->ResolveCollision(*circles[j]);
			}
		}
	}

	// オブジェクトの更新
	for (auto& o : objects) {
		o->Update();
	}

	if (lookAtObject) {

		if (selectedObject && selectedObject != camera) {
			Vector3 newPos;
			auto circle = dynamic_cast<Circle*>(selectedObject.get());

			// 追従対象からカメラまでのオフセット
			Vector3 offset = {0.0f, 0.0f , circle->GetRadius() - 30.0f};

			const Mat4 x = Mat4::RotateX(viewProjection_.rotation_.x);
			const Mat4 y = Mat4::RotateY(viewProjection_.rotation_.y);

			// オフセットをカメラの回転に合わせて回転させる
			Vector3 newOffset = TransformNormal(offset, x * y);
			for (int i = 0; i < 3; ++i) {
				offset[i] = newOffset[i];
			}

			// 座標をコピーしてオフセット分ずらす
			for (int i = 0; i < 3; ++i) {
				newPos[i] = selectedObject.get()->GetWorldTransform()->translation_[i] + offset[i];
			}

			camera->SetTransform(
				newPos.ConvertToVec3(),
				camera->GetTransform().rotation,
				Vec3::one
			);
		}
	} else {
		camera->SetTransform(
			camera->GetTransform().position,
			camera->GetTransform().rotation,
			camera->GetTransform().scale
		);
	}

	camera->Update();

	viewProjection_.matView = camera->GetViewProjection()->matView;
	viewProjection_.matProjection = camera->GetViewProjection()->matProjection;
	viewProjection_.TransferMatrix();


	ImGui::Begin("Outliner");

	for (const auto& o : objects) {
		RenderOutliner(o, selectedObject);
	}

	ImGui::End();

	ImGui::Begin("Details");

	if (ImGui::BeginTabBar("Object")) {
		if (ImGui::BeginTabItem("Object")) {
			if (selectedObject != nullptr) {
				selectedObject->Details();
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("World")) {
			ImGui::DragFloat("Gravity", &gravity, 1.0f);
			ImGui::Checkbox("Look at Object", &lookAtObject);
			ImGui::Checkbox("DrawDebug", &bDrawDebug);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>


	for (const auto& o : objects) {
		o->Draw(viewProjection_);
	}

	for (const auto& o : objects) {
		if (auto circle = dynamic_cast<Circle*>(o.get())) {
			circle->DebugDraw();
		}
	}

	DrawGrid();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void RenderOutliner(const std::shared_ptr<Object>& object, std::shared_ptr<Object>& selectedObject) {
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (object->GetChildren().empty()) {
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (object == selectedObject) {
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}

	bool nodeOpen = ImGui::TreeNodeEx(object->GetName().c_str(), nodeFlags);

	if (ImGui::IsItemClicked()) {
		selectedObject = object;
	}

	if (nodeOpen && !object->GetChildren().empty()) {
		for (const auto& child : object->GetChildren()) {
			RenderOutliner(child, selectedObject);
		}
		ImGui::TreePop();
	} else if (nodeOpen) {
		ImGui::TreePop();
	}
}

Vector3 TransformNormal(const Vector3 v, const Mat4& m) {
	return {
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0], v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	};
}

void DrawGrid() {
	const float kGridHalfWidth = 100.0f; // Gridの半分の幅
	const uint32_t kSubdivision = 50; // 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / static_cast<float>(kSubdivision); // ひとつ分の長さ

	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		// 上の情報を使ってワールド座標系上の視点と終点を求める
		float x = kGridHalfWidth - xIndex * kGridEvery;
		Vector3 start = {x, 0, -kGridHalfWidth};
		Vector3 end = {x, 0, kGridHalfWidth};
		// 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、何でも良い
		if (xIndex == kSubdivision / 2) {
			PrimitiveDrawer::GetInstance()->DrawLine3d(start, end, {0.125f,0.125f,0.5f,1.0f});
		} else {
			PrimitiveDrawer::GetInstance()->DrawLine3d(start, end, {0.75f,0.75f,0.75f,1.0f});
		}
	}

	// 左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHalfWidth + zIndex * kGridEvery;
		Vector3 start = {-kGridHalfWidth, 0, z};
		Vector3 end = {kGridHalfWidth, 0, z};
		if (zIndex == kSubdivision / 2) {
			PrimitiveDrawer::GetInstance()->DrawLine3d(start, end, {0.5f,0.125f,0.125f,1.0f});
		} else {
			PrimitiveDrawer::GetInstance()->DrawLine3d(start, end, {0.75f,0.75f,0.75f,1.0f});
		}
	}
}