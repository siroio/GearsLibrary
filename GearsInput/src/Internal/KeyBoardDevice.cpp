#include <Internal/KeyBoardDevice.h>
#include <WinUser.h>

Glib::Internal::Input::KeyBoardDevice::~KeyBoardDevice()
{
    rawInputDevice_->dwFlags = RIDEV_REMOVE;
    RegisterRawInputDevices(rawInputDevice_.get(), 1, sizeof(RAWINPUTDEVICE));
    rawInputDevice_.reset();
}

bool Glib::Internal::Input::KeyBoardDevice::Initialize()
{
    // キーボード取得のデバイスを作成
    rawInputDevice_ = std::make_unique<RAWINPUTDEVICE>(0x01, 0x06, 0, nullptr);
    // デバイスの登録
    if (!RegisterRawInputDevices(rawInputDevice_.get(), 1, sizeof(RAWINPUTDEVICE))) return false;

    // メッセージ取得用のプロシージャ登録
    Window::RegisterProcedure(this);

    return true;
}

void Glib::Internal::Input::KeyBoardDevice::Update()
{
    prevKeyState_ = currentKeyState_;

    // バッファから入力を取得
    for (int i = 0; i < currentKeyState_.size(); i++)
    {
        currentKeyState_[i] = frameBuffer_[i] ? 0x10 : 0x00;
    }
}

bool Glib::Internal::Input::KeyBoardDevice::GetKey(KeyCode key) const
{
    return currentKeyState_.at(static_cast<unsigned char>(key)) & 0x10;
}

bool Glib::Internal::Input::KeyBoardDevice::GetKeyDown(KeyCode key) const
{
    unsigned char prevState = ~(prevKeyState_.at(static_cast<unsigned char>(key)) & 0x10);
    unsigned char currentState = currentKeyState_.at(static_cast<unsigned char>(key)) & 0x10;
    return prevState & currentState;
}

bool Glib::Internal::Input::KeyBoardDevice::GetKeyUp(KeyCode key) const
{
    unsigned char prevState = prevKeyState_.at(static_cast<unsigned char>(key)) & 0x10;
    unsigned char currentState = ~(currentKeyState_.at(static_cast<unsigned char>(key)) & 0x10);
    return prevState & currentState;
}

void Glib::Internal::Input::KeyBoardDevice::ProcessKeyboard(const HRAWINPUT* hRawInput)
{
    UINT size = 0;
    GetRawInputData(*hRawInput, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
    auto buffer = std::make_unique<BYTE[]>(size);
    if (GetRawInputData(*hRawInput, RID_INPUT, buffer.get(), &size, sizeof(RAWINPUTHEADER)) != size)
    {
        return;
    }

    // バッファに追加
    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.get());
    if (raw->header.dwType == RIM_TYPEKEYBOARD)
    {
        auto key = raw->data.keyboard.VKey;
        auto flags = raw->data.keyboard.Flags;
        frameBuffer_[key] = !(flags & RI_KEY_BREAK);
    }
}

void Glib::Internal::Input::KeyBoardDevice::operator()(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg != WM_INPUT) return;
    auto inputData = reinterpret_cast<HRAWINPUT>(lparam);
    ProcessKeyboard(&inputData);
}
