#include <Vector2.h>
#include <Vector3.h>
#include <Vector4.h>
#include <Matrix4x4.h>
#include <Quaternion.h>
#include <Color.h>
#include <iostream>
#include <Random.h>

#include <string>
#include <CSVLoader.h>

auto main() -> int
{
    Vector2 vec2;
    std::cout << "Vector2\n" << vec2 << std::endl;

    Vector3 vec3;
    std::cout << "Vector3\n" << vec3 << std::endl;

    Vector4 vec4;
    std::cout << "Vector4\n" << vec4 << std::endl;

    Matrix4x4 mat4x4;
    std::cout << "Matrix4x4\n" << mat4x4 << std::endl;

    Quaternion q;
    std::cout << "Quaternion\n" << q << std::endl;

    Color c;
    std::cout << "Color\n" << c << std::endl;

    // 単位クォータニオンの生成
    Quaternion identity = Quaternion::Identity();

    // (0,0,0)を回転させないテスト
    Vector3 v1(0.0f, 0.0f, 0.0f);
    Quaternion q1 = Quaternion::AngleAxis(45.0f, Vector3::Up());
    Vector3 result1 = q1 * v1;
    if (result1 == v1)
        std::cout << "(0,0,0) rotation test: PASS" << std::endl;
    else
        std::cout << "(0,0,0) rotation test: FAIL" << std::endl;

    // (1,0,0)をx軸周りに90度回転させるテスト
    Vector3 v2(1.0f, 0.0f, 0.0f);
    Quaternion q2 = Quaternion::AngleAxis(90.0f, Vector3::Right());
    Vector3 result2 = q2 * v2;
    if (result2 == Vector3::Forward())
        std::cout << "(1,0,0) rotation test: PASS" << std::endl;
    else
        std::cout << "(1,0,0) rotation test: FAIL" << std::endl;

    // (0,1,0)をy軸周りに90度回転させるテスト
    Vector3 v3(0.0f, 1.0f, 0.0f);
    Quaternion q3 = Quaternion::AngleAxis(90.0f, Vector3::Up());
    Vector3 result3 = q3 * v3;
    if (result3 == Vector3::Forward())
        std::cout << "(0,1,0) rotation test: PASS" << std::endl;
    else
        std::cout << "(0,1,0) rotation test: FAIL" << std::endl;

    // (0,0,1)をz軸周りに90度回転させるテスト
    Vector3 v4(0.0f, 0.0f, 1.0f);
    Quaternion q4 = Quaternion::AngleAxis(90.0f, Vector3::Forward());
    Vector3 result4 = q4 * v4;
    if (result4 == Vector3::Up())
        std::cout << "(0,0,1) rotation test: PASS" << std::endl;
    else
        std::cout << "(0,0,1) rotation test: FAIL" << std::endl;

    return 0;
}
