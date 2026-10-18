/**
* @file Ext02-Matrix4x4_fix_test.cpp
* @brief Matrix4x4 单元测试
* @details
* 对 Fern::Matrix4x4 的公开接口做可自动判定的单元测试，覆盖：
*   - 构造与单位矩阵
*   - 加减法运算符的返回值（回归"运算符缺 return"导致的未定义行为）
*   - 转置 operator!
*   - 乘法约定：operator* 为 A·this（左乘），operator^ 为 this·A（右乘）
*   - 仿射求逆 operator~，以及它在非仿射 / 奇异输入下的静默行为
*   - 点变换 operator* 与方向变换 operator^ 的区别
*   - 各静态工厂方法
*
* 用例分两类：
*   [验证] 断言"数学上应当成立"的性质；
*   [记录] 固化"当前不理想、但本轮不改"的既有行为（哨兵返回值、静默返回单位阵）。
*          将来若把这些行为改成抛异常 / 返回 optional，这些用例会先变红，
*          起到护栏作用。
*
* 本文件由 AI 辅助生成，用例设计与断言口径经人工核对后采用。
* @author DeepSeek
* @date 2026/10/18
*/

// 只依赖数学三件套，不引入 EasyX，因此本测试是纯控制台程序
//
// 注意：Vector3D.h 与 Matrix4x4.h 内部使用了 std::ostream 与 std::string，
// 但自身没有 #include 对应头文件，依赖调用方先引入。
// 引擎的聚合头 Fern3DGameEngine.h 在 L17 起已经引入了这些标准库，所以正常构建无感；
// 此处为了让本测试不依赖聚合头（从而不依赖 EasyX），显式先补齐。
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>

#include "FernPack/basic_funcion/MathUtils.h"
#include "FernPack/math/Vector3D.h"
#include "FernPack/math/Matrix4x4.h"

#include <iostream>
#include <iomanip>
#include <cmath>

using Fern::Matrix4x4;
using Fern::Vector3D;

// ==================== 极简断言层 ====================

static int g_pass = 0;			// 累计通过
static int g_fail = 0;			// 累计失败
static int g_blockFail = 0;		// 当前测试块内的失败数

static void ResetBlock() { g_blockFail = 0; }

static void Check(bool ok, const char* expr, int line) {
	if (ok) {
		g_pass++;
	}
	else {
		g_fail++;
		g_blockFail++;
		std::cout << "    [FAIL] 第 " << line << " 行: " << expr << std::endl;
	}
}

static void CheckNear(double a, double b, double eps, const char* expr, int line) {
	Check(Fern::Math::isNearlyEqual(a, b, eps), expr, line);
}

#define CHECK(expr)					Check((expr), #expr, __LINE__)
#define CHECK_NEAR(a, b, eps)		CheckNear((a), (b), (eps), #a " 约等于 " #b, __LINE__)
#define CHECK_VEC(v, ex, ey, ez, eps)					\
	do {												\
		CheckNear((v).x, (ex), (eps), #v ".x", __LINE__);	\
		CheckNear((v).y, (ey), (eps), #v ".y", __LINE__);	\
		CheckNear((v).z, (ez), (eps), #v ".z", __LINE__);	\
	} while (0)

static bool MatEqual(const Matrix4x4& a, const Matrix4x4& b, double eps = 1e-9) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (!Fern::Math::isNearlyEqual(a.values[i][j], b.values[i][j], eps))
				return false;
	return true;
}

static void PrintMat(const Matrix4x4& m) {
	for (int i = 0; i < 4; i++) {
		std::cout << "    |";
		for (int j = 0; j < 4; j++)
			std::cout << std::setw(10) << m.values[i][j] << " ";
		std::cout << "|" << std::endl;
	}
}

static void EndBlock() {
	if (g_blockFail == 0)
		std::cout << "  结果: v 通过" << std::endl << std::endl;
	else
		std::cout << "  结果: x 失败（本组 " << g_blockFail << " 项）" << std::endl << std::endl;
}

// ==================== 测试 1：构造与单位矩阵 ====================

void Test01_ConstructAndIdentity() {
	std::cout << "========== 测试1：构造与单位矩阵 ==========" << std::endl;
	ResetBlock();

	Matrix4x4 def;									// 默认参数
	Matrix4x4 uni(MATRIX_INIT_WITH_UNIT_MATRIX);
	Matrix4x4 idf = Matrix4x4::Identity();

	CHECK(MatEqual(def, uni));
	CHECK(MatEqual(def, idf));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			CHECK_NEAR(def.values[i][j], (i == j ? 1.0 : 0.0), 1e-12);

	Matrix4x4 zero(MATRIX_INIT_WITH_ALL_0);
	Matrix4x4 one(MATRIX_INIT_WITH_ALL_1);
	CHECK_NEAR(zero.values[0][0], 0.0, 1e-12);
	CHECK_NEAR(zero.values[3][3], 0.0, 1e-12);
	CHECK_NEAR(one.values[0][0], 1.0, 1e-12);
	CHECK_NEAR(one.values[3][3], 1.0, 1e-12);

	std::cout << "  单位矩阵:" << std::endl;
	PrintMat(def);

	EndBlock();
}

// ==================== 测试 2：加减法运算符的返回值 ====================
// 回归：operator- / operator+ / operator+= 曾经缺少 return，
// 取用返回值属于未定义行为。此处直接消费返回值，若缺 return 会立刻暴露。

void Test02_AddSubReturnValue() {
	std::cout << "========== 测试2：加减法运算符的返回值（回归） ==========" << std::endl;
	ResetBlock();

	Matrix4x4 a = Matrix4x4::Translation(1.0, 2.0, 3.0);
	Matrix4x4 b = Matrix4x4::Translation(10.0, 20.0, 30.0);

	Matrix4x4 sum = a + b;							// 消费返回值
	Matrix4x4 dif = a - b;

	CHECK_NEAR(sum.values[0][3], 11.0, 1e-12);
	CHECK_NEAR(sum.values[1][3], 22.0, 1e-12);
	CHECK_NEAR(sum.values[2][3], 33.0, 1e-12);

	CHECK_NEAR(dif.values[0][3], -9.0, 1e-12);
	CHECK_NEAR(dif.values[1][3], -18.0, 1e-12);
	CHECK_NEAR(dif.values[2][3], -27.0, 1e-12);

	// 对角线（旋转 / 缩放部分）逐元素相加减
	CHECK_NEAR(sum.values[0][0], 2.0, 1e-12);		// 1 + 1
	CHECK_NEAR(dif.values[0][0], 0.0, 1e-12);		// 1 - 1
	CHECK_NEAR(sum.values[3][3], 2.0, 1e-12);
	CHECK_NEAR(dif.values[3][3], 0.0, 1e-12);

	// 复合赋值
	Matrix4x4 c = a;
	c += b;
	CHECK_NEAR(c.values[1][3], 22.0, 1e-12);

	Matrix4x4 d = a;
	d -= b;
	CHECK_NEAR(d.values[1][3], -18.0, 1e-12);

	EndBlock();
}

// ==================== 测试 3：转置 operator! ====================

void Test03_Transpose() {
	std::cout << "========== 测试3：转置 operator! ==========" << std::endl;
	ResetBlock();

	Matrix4x4 m = Matrix4x4::RotationY(Fern::Math::PIHALF);
	m.values[0][3] = 5.0;			// 手工塞入平移，使矩阵非对称
	m.values[1][3] = 6.0;
	m.values[2][3] = 7.0;

	Matrix4x4 mt = !m;				// operator! 非 const，必须作用于具名左值
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			CHECK_NEAR(mt.values[i][j], m.values[j][i], 1e-12);

	// 转置两次回到自身
	Matrix4x4 mtt = !mt;
	CHECK(MatEqual(mtt, m, 1e-12));

	// 平移原本在第 4 列，转置后应落到第 4 行
	CHECK_NEAR(mt.values[3][0], 5.0, 1e-12);
	CHECK_NEAR(mt.values[3][1], 6.0, 1e-12);
	CHECK_NEAR(mt.values[3][2], 7.0, 1e-12);
	// 底行不再是 (0,0,0,1)，此后对该转置矩阵求仿射逆应失败
	CHECK(!Fern::Math::isNearlyEqual(mt.values[3][0], 0.0, 1e-9));

	EndBlock();
}

// ==================== 测试 4：operator* 是左乘，书写顺序 == 应用顺序 ====================
// a * b 内部计算 b·a。作用于列向量 v 时：(b·a)·v = b·(a·v)，
// 即 a 先作用、b 后作用 —— 书写顺序与应用顺序一致。

void Test04_MultiplyIsLeftMultiply() {
	std::cout << "========== 测试4：operator* 左乘约定 ==========" << std::endl;
	ResetBlock();

	Vector3D origin(0.0, 0.0, 0.0);
	Matrix4x4 R = Matrix4x4::RotationY(Fern::Math::PIHALF);
	Matrix4x4 T = Matrix4x4::Translation(1.0, 0.0, 0.0);

	// R * T 计算 T·R：先旋转后平移。原点先被旋转（仍为原点），再被平移
	Vector3D a = (R * T) * origin;
	CHECK_VEC(a, 1.0, 0.0, 0.0, 1e-9);

	// T * R 计算 R·T：先平移后旋转。原点先被平移到 (1,0,0)，再旋转
	Vector3D b = (T * R) * origin;
	CHECK_NEAR(b.y, 0.0, 1e-9);
	CHECK_NEAR(std::fabs(b.z), 1.0, 1e-9);			// 绕 Y 轴 90 度后落到 Z 轴
	CHECK(!(a == b));								// 两种顺序结果必然不同

	std::cout << "  先R后T，原点 -> (" << a.x << ", " << a.y << ", " << a.z << ")" << std::endl;
	std::cout << "  先T后R，原点 -> (" << b.x << ", " << b.y << ", " << b.z << ")" << std::endl;

	EndBlock();
}

// ==================== 测试 5：operator^ 是右乘 ====================

void Test05_CaretIsRightMultiply() {
	std::cout << "========== 测试5：operator^ 右乘约定 ==========" << std::endl;
	ResetBlock();

	Vector3D origin(0.0, 0.0, 0.0);
	Matrix4x4 R = Matrix4x4::RotationY(Fern::Math::PIHALF);
	Matrix4x4 T = Matrix4x4::Translation(1.0, 0.0, 0.0);

	Vector3D viaStar = (T * R) * origin;			// R·T
	Vector3D viaCaret = (T ^ R) * origin;			// T·R

	// T ^ R 计算 T·R：先旋转后平移
	CHECK_VEC(viaCaret, 1.0, 0.0, 0.0, 1e-9);
	CHECK(!(viaStar == viaCaret));					// * 与 ^ 语义不同

	// 同型矩阵应满足：a * b == b ^ a
	Matrix4x4 left = T * R;							// R·T
	Matrix4x4 right = R ^ T;						// R·T
	CHECK(MatEqual(left, right, 1e-12));

	EndBlock();
}

// ==================== 测试 6：仿射求逆 operator~ ====================

void Test06_AffineInverse() {
	std::cout << "========== 测试6：仿射求逆 operator~ ==========" << std::endl;
	ResetBlock();

	Matrix4x4 S = Matrix4x4::Scale(2.0, 3.0, 4.0);
	Matrix4x4 R = Matrix4x4::RotationY(Fern::Math::PIHALF);
	Matrix4x4 T = Matrix4x4::Translation(5.0, -2.0, 7.0);
	Matrix4x4 M = T * R * S;						// 书写顺序 = 先 S 再 R 再 T

	Matrix4x4 inv = ~M;

	CHECK(MatEqual(M * inv, Matrix4x4::Identity(), 1e-9));
	CHECK(MatEqual(inv * M, Matrix4x4::Identity(), 1e-9));

	// 逆应当把点搬回去
	Vector3D p(1.0, 2.0, 3.0);
	Vector3D q = M * p;
	Vector3D back = inv * q;
	CHECK_VEC(back, 1.0, 2.0, 3.0, 1e-9);

	std::cout << "  M * ~M 与单位矩阵一致: " << (MatEqual(M * inv, Matrix4x4::Identity(), 1e-9) ? "是" : "否") << std::endl;

	EndBlock();
}

// ==================== 测试 7：[记录] 非仿射矩阵求逆 ====================

void Test07_InverseOfNonAffine() {
	std::cout << "========== 测试7：[记录] 非仿射矩阵求逆 ==========" << std::endl;
	ResetBlock();

	Matrix4x4 m = Matrix4x4::Translation(1.0, 2.0, 3.0);
	m.values[3][3] = 2.0;			// 底行不再是 (0,0,0,1)

	Matrix4x4 inv = ~m;

	// [记录] 当前行为：静默返回单位矩阵。
	// 调用方无法区分"逆恰好是单位阵"和"求逆失败"。
	// 若将来改成抛异常或返回 optional，此用例会变红 —— 这是预期行为。
	CHECK(MatEqual(inv, Matrix4x4::Identity(), 1e-12));

	std::cout << "  当前行为: 底行为 (" << m.values[3][0] << ", " << m.values[3][1]
		<< ", " << m.values[3][2] << ", " << m.values[3][3] << ") -> 返回单位矩阵" << std::endl;

	EndBlock();
}

// ==================== 测试 8：[记录] 奇异矩阵求逆 ====================

void Test08_InverseOfSingular() {
	std::cout << "========== 测试8：[记录] 奇异矩阵求逆 ==========" << std::endl;
	ResetBlock();

	Matrix4x4 m = Matrix4x4::Scale(0.0, 1.0, 1.0);	// 3x3 部分行列式为 0
	Matrix4x4 inv = ~m;

	// [记录] 当前行为：同样静默返回单位矩阵
	CHECK(MatEqual(inv, Matrix4x4::Identity(), 1e-12));

	EndBlock();
}

// ==================== 测试 9：点变换 operator*(Vector3D) ====================

void Test09_PointTransform() {
	std::cout << "========== 测试9：点变换 operator*(Vector3D) ==========" << std::endl;
	ResetBlock();

	Matrix4x4 T = Matrix4x4::Translation(1.0, 2.0, 3.0);
	Vector3D p(10.0, 20.0, 30.0);
	Vector3D r = T * p;

	// 点会被平移
	CHECK_VEC(r, 11.0, 22.0, 33.0, 1e-9);

	// 缩放在点上生效
	Matrix4x4 S = Matrix4x4::Scale(2.0, 3.0, 4.0);
	Vector3D s = S * Vector3D(1.0, 1.0, 1.0);
	CHECK_VEC(s, 2.0, 3.0, 4.0, 1e-9);

	std::cout << "  平移作用在点 (10,20,30) 上 -> (" << r.x << ", " << r.y << ", " << r.z << ")" << std::endl;

	EndBlock();
}

// ==================== 测试 10：方向变换 operator^(Vector3D) ====================

void Test10_DirectionTransform() {
	std::cout << "========== 测试10：方向变换 operator^(Vector3D) ==========" << std::endl;
	ResetBlock();

	Matrix4x4 T = Matrix4x4::Translation(1.0, 2.0, 3.0);
	Vector3D d(10.0, 20.0, 30.0);
	Vector3D r = T ^ d;

	// 方向不受平移影响
	CHECK_VEC(r, 10.0, 20.0, 30.0, 1e-9);

	// 但旋转对方向生效
	Matrix4x4 R = Matrix4x4::RotationY(Fern::Math::PIHALF);
	Vector3D rd = R ^ Vector3D(1.0, 0.0, 0.0);
	CHECK_NEAR(rd.y, 0.0, 1e-9);
	CHECK_NEAR(std::fabs(rd.z), 1.0, 1e-9);

	std::cout << "  平移作用在方向 (10,20,30) 上 -> (" << r.x << ", " << r.y << ", " << r.z << ")" << std::endl;

	EndBlock();
}

// ==================== 测试 11：[记录] w 接近 0 时的点变换 ====================

void Test11_PointTransformWhenWNearZero() {
	std::cout << "========== 测试11：[记录] w 接近 0 时的点变换 ==========" << std::endl;
	ResetBlock();

	Matrix4x4 m(MATRIX_INIT_WITH_ALL_0);			// 底行全 0 => w = 0
	Vector3D p(1.0, 2.0, 3.0);
	Vector3D r = m * p;

	// [记录] 当前行为：返回哨兵向量 (114, 514, 1919)，而不是报错或返回原点。
	// 这是一个"魔法值"，调用方需自行识别；将来若改为抛异常，此用例会变红。
	CHECK_VEC(r, 114.0, 514.0, 1919.0, 1e-9);

	std::cout << "  当前行为: w 约等于 0 -> (" << r.x << ", " << r.y << ", " << r.z << ")" << std::endl;

	EndBlock();
}

// ==================== 测试 12：旋转矩阵的正交性 ====================

void Test12_RotationOrthogonality() {
	std::cout << "========== 测试12：旋转矩阵正交性 R * ~R = I ==========" << std::endl;
	ResetBlock();

	const double angle = 0.7;
	Matrix4x4 rx = Matrix4x4::RotationX(angle);
	Matrix4x4 ry = Matrix4x4::RotationY(angle);
	Matrix4x4 rz = Matrix4x4::RotationZ(angle);

	CHECK(MatEqual(rx * ~rx, Matrix4x4::Identity(), 1e-9));
	CHECK(MatEqual(ry * ~ry, Matrix4x4::Identity(), 1e-9));
	CHECK(MatEqual(rz * ~rz, Matrix4x4::Identity(), 1e-9));

	// 旋转的逆应等于转置（正交矩阵性质），借此交叉验证 ! 与 ~ 的一致性
	Matrix4x4 R = Matrix4x4::RotationX(angle);
	Matrix4x4 inv = ~R;
	Matrix4x4 trs = !R;
	CHECK(MatEqual(inv, trs, 1e-9));

	EndBlock();
}

// ==================== 测试 13：绕任意点旋转 ====================

void Test13_RotationAroundOriginKeepPoint() {
	std::cout << "========== 测试13：绕点旋转不移动该点 ==========" << std::endl;
	ResetBlock();

	Vector3D o(3.0, -4.0, 5.0);

	Matrix4x4 rx = Matrix4x4::RotationAroundOriginX(Fern::Math::PIHALF, o);
	Matrix4x4 ry = Matrix4x4::RotationAroundOriginY(Fern::Math::PIHALF, o);
	Matrix4x4 rz = Matrix4x4::RotationAroundOriginZ(Fern::Math::PIHALF, o);

	// pivot 自身在旋转下是不动点
	Vector3D px = rx * o;
	Vector3D py = ry * o;
	Vector3D pz = rz * o;

	CHECK_VEC(px, 3.0, -4.0, 5.0, 1e-9);
	CHECK_VEC(py, 3.0, -4.0, 5.0, 1e-9);
	CHECK_VEC(pz, 3.0, -4.0, 5.0, 1e-9);

	// 不同于原点旋转：同一角度绕点旋转，普通点会被搬走
	Vector3D q(4.0, -4.0, 5.0);
	Vector3D qx = rx * q;
	CHECK(!(qx == q));

	std::cout << "  pivot (" << o.x << ", " << o.y << ", " << o.z << ") 绕自身旋转后不变" << std::endl;

	EndBlock();
}

// ==================== 测试 14：乘法结合律与单位元 ====================

void Test14_AssociativityAndIdentity() {
	std::cout << "========== 测试14：结合律与单位元 ==========" << std::endl;
	ResetBlock();

	Matrix4x4 T = Matrix4x4::Translation(1.0, 2.0, 3.0);
	Matrix4x4 R = Matrix4x4::RotationY(0.5);
	Matrix4x4 S = Matrix4x4::Scale(2.0, 2.0, 2.0);
	Matrix4x4 I = Matrix4x4::Identity();

	// 单位元
	CHECK(MatEqual(T * I, T, 1e-12));
	CHECK(MatEqual(I * T, T, 1e-12));

	// 结合律：(T*R)*S 与 T*(R*S)
	Matrix4x4 lhs = (T * R) * S;
	Matrix4x4 rhs = T * (R * S);
	CHECK(MatEqual(lhs, rhs, 1e-9));

	// 作用在同一个点上结果也应一致
	Vector3D p(1.0, 1.0, 1.0);
	Vector3D a = lhs * p;
	Vector3D b = rhs * p;
	CHECK(a == b);

	EndBlock();
}

// ==================== 入口 ====================

int main() {
	std::cout << std::setprecision(6) << std::fixed;
	std::cout << "Fern3DGameEngine : Matrix4x4 单元测试" << std::endl;
	std::cout << "=====================================" << std::endl << std::endl;

	Test01_ConstructAndIdentity();
	Test02_AddSubReturnValue();
	Test03_Transpose();
	Test04_MultiplyIsLeftMultiply();
	Test05_CaretIsRightMultiply();
	Test06_AffineInverse();
	Test07_InverseOfNonAffine();
	Test08_InverseOfSingular();
	Test09_PointTransform();
	Test10_DirectionTransform();
	Test11_PointTransformWhenWNearZero();
	Test12_RotationOrthogonality();
	Test13_RotationAroundOriginKeepPoint();
	Test14_AssociativityAndIdentity();

	std::cout << "=====================================" << std::endl;
	std::cout << "合计: 通过 " << g_pass << " 项, 失败 " << g_fail << " 项" << std::endl;
	if (g_fail == 0) {
		std::cout << "全部通过" << std::endl;
		return 0;
	}
	std::cout << "存在失败项" << std::endl;
	return 1;			// 非 0 退出码，便于外部脚本 / 后续接入 CI 判定
}
