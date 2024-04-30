#include "LineTool.hpp"

#include "raylib.h"
#include "raymath.h"

#include "Algorithms.hpp"
#include "Project.hpp"
#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "Cursor.hpp"

LineTool::LineTool(Project* project, ColorSystem* colorSystem) : 
    m_Project(project),
    m_ColorSystem(colorSystem),
    m_PointA(Vector2Zero()), m_PointB(Vector2Zero()) { }

void LineTool::OnButtonPress() {
    m_PointA = m_Project->canvas->PositionInWorldSpace(GetMousePosition(), m_Project->camera);
}

void LineTool::OnButtonDown() { 
    m_PointB = m_Project->canvas->PositionInWorldSpace(GetMousePosition(), m_Project->camera);
}

void LineTool::OnButtonRelease() {
    int ax = m_Project->canvas->PositionAsCanvasIndex(m_PointA).x;
    int ay = m_Project->canvas->PositionAsCanvasIndex(m_PointA).y;

    int bx = m_Project->canvas->PositionAsCanvasIndex(m_PointB).x;
    int by = m_Project->canvas->PositionAsCanvasIndex(m_PointB).y;

    if(!m_Project->canvas->CanvasIndexValid({ static_cast<float>(ax), static_cast<float>(ay) }) || !m_Project->canvas->CanvasIndexValid({ static_cast<float>(bx), static_cast<float>(by) })) {
        return;
    }

    Algorithms::DigitalDifferentialAnalyzer(ax, ay, bx, by, m_ColorSystem->GetColor(), *m_Project->canvas->layerSystem.GetLayer());

    TraceLog(
        LOG_INFO, 
        TextFormat(
            "Line drawn for positions: x1.:%i, y1.:%i, x2.:%i, y2.:%i",
            ax, ay,
            bx, by
        )
    );

    m_PointA = Vector2Zero();
    m_PointB = Vector2Zero();

    m_Project->canvas->ToggleTextureReload();
}

void LineTool::Render() {
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        m_Project->canvas->DrawCell(
            (m_Project->canvas->PositionAsCanvasCell(m_PointA).x) * m_Project->canvas->scale,
            (m_Project->canvas->PositionAsCanvasCell(m_PointA).y) * m_Project->canvas->scale,
            1.0f,
            RED
        );

        m_Project->canvas->DrawCell(
            (m_Project->canvas->PositionAsCanvasCell(m_PointB).x) * m_Project->canvas->scale,
            (m_Project->canvas->PositionAsCanvasCell(m_PointB).y) * m_Project->canvas->scale,
            1.0f,
            RED
        );

        DrawLine(
            (m_Project->canvas->PositionAsCanvasCell(m_PointA).x + m_Project->canvas->GetCellSize().x / 2.0f) * m_Project->canvas->scale, 
            (m_Project->canvas->PositionAsCanvasCell(m_PointA).y + m_Project->canvas->GetCellSize().y / 2.0f) * m_Project->canvas->scale, 
            (m_Project->canvas->PositionAsCanvasCell(m_PointB).x + m_Project->canvas->GetCellSize().x / 2.0f) * m_Project->canvas->scale, 
            (m_Project->canvas->PositionAsCanvasCell(m_PointB).y + m_Project->canvas->GetCellSize().y / 2.0f) * m_Project->canvas->scale, 
            RED
        );
    }
}
