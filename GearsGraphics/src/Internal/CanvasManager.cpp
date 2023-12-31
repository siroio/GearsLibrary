#include <Internal/CanvasManager.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/ImGuiManager.h>
#include <Internal/UIRenderer.h>
#include <Components/Canvas.h>
#include <unordered_map>
#include <map>
#include <list>
#include <ranges>

namespace
{
    // キャンバス一覧
    std::map<int, std::list<Glib::WeakPtr<Glib::Canvas>>> s_canvas;

    // キャンバス内のレンダラー一覧
    std::unordered_map<std::uintptr_t, std::list<Glib::WeakPtr<Glib::Internal::UIRenderer>>> s_renderers;

    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_imgui = Glib::Internal::Debug::ImGuiManager::Instance();
}

void Glib::Internal::Graphics::CanvasManager::Draw()
{
#if defined(DEBUG) || defined(_DEBUG)
    s_imgui->SetRenderTarget();
#else
    s_dx12->SetDefaultRenderTarget();
#endif
    for (const auto& canvsList : s_canvas | std::ranges::views::values)
    {
        for (const auto& canvas : canvsList)
        {
            DrawUI(canvas);
        }
    }
}

void Glib::Internal::Graphics::CanvasManager::AddCanvas(const WeakPtr<Canvas>& canvas)
{
    s_canvas[canvas->Order()].push_back(canvas);
}

void Glib::Internal::Graphics::CanvasManager::AddUI(const WeakPtr<Internal::UIRenderer>& renderer, const WeakPtr<Canvas>& canvas)
{
    if (canvas.expired()) return;
    s_renderers[canvas.getId()].push_back(renderer);
}

void Glib::Internal::Graphics::CanvasManager::ChangeCanvasOrder(const WeakPtr<Canvas>& canvas, int order)
{
    if (s_renderers.contains(canvas.getId())) return;
    // 新しい順序に変更
    s_canvas[canvas->Order()].remove(canvas);
    s_canvas[order].push_back(canvas);
}

void Glib::Internal::Graphics::CanvasManager::DrawUI(const WeakPtr<Canvas>& canvas) const
{
    if (canvas.expired())
    {
        //無効なキャンバスを削除
        s_renderers.erase(canvas.getId());
    }
    else if (canvas->Active())
    {
        // 有効なUIを描画
        for (const auto& renderer : s_renderers[canvas.getId()])
        {
            renderer->DrawUI();
        }
    }
}
