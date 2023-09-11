#include "MapDataExtra.h"

#include "../Macro.h"
#include "../FA2Expand.h"
#include "../Meta/INIMeta.h"

#include <CFinalSunDlg.h>
#include <CMapData.h>
#include <memory>

const char* retriveLowChar(const TCHAR* raw)
{
	return reinterpret_cast<const char*>(raw);
}

DEFINE_HOOK(4B5460, CMapData_InitializeBuildingTypes, 7)
{
	GET(CMapData*, pThis, ECX);
	GET_STACK(const char*, ID, 0x4);

	auto ProcessType = [pThis](const char* ID) {
		int idx = pThis->GetBuildingTypeID(ID);
		auto& DataExt = CMapDataExt::BuildingDataExts[idx];
		FA2::CString ImageID = INIMeta::GetRules().GetString(ID, "Image");
		if (ImageID.GetLength() == 0) {
			ImageID = ID;
		}

		auto foundation = GlobalVars::INIFiles::Art->GetString(ImageID, "Foundation");
		foundation.Trim();
		// original way compatible
		if (0 != _strcmpi(foundation, "Custom")) {
			// 1x1 1x2 3x3
			if (foundation.GetLength() < 3) {
				return;
			}
			auto const rawStr = reinterpret_cast<const char*>(foundation.operator LPCTSTR());
			DataExt.Width = std::max(atoi(&rawStr[0]), 1);
			DataExt.Height = std::max(atoi(&rawStr[2]), 1);
			return;
		}
		// Custom, code reference Ares
		DataExt.Width = GlobalVars::INIFiles::Art->GetInteger(ImageID, "Foundation.X", 0);
		DataExt.Height = GlobalVars::INIFiles::Art->GetInteger(ImageID, "Foundation.Y", 0);
		DataExt.Foundations = std::make_unique<BuildingDataExt::FoundationStorage>();
		auto ParsePoint = [](const char* str) {
			int x = 0, y = 0;
			switch (sscanf_s(str, "%d,%d", &x, &y)) {
				case 0:
					x = 0;
					y = 0;
					break;
				case 1:
					y = 0;
					break;
				case 2:
					break;
				default:
					__assume(0);
			}
			return MapCoord{ x,y };
		};
		for (int i = 0; i < DataExt.Width * DataExt.Height; ++i) {
			FA2::CString key;
			key.Format("Foundation.%d", i);
			auto const point = GlobalVars::INIFiles::Art->GetString(ImageID, key);
			if (point.GetLength() == 0) {
				break;
			}
			DataExt.Foundations->push_back(ParsePoint(point));
		}

		// Build outline draw data
		DataExt.LinesToDraw = std::make_unique<BuildingDataExt::FoundationLineStorage>();
		std::vector<std::vector<bool>> LinesX, LinesY;

		LinesX.resize(DataExt.Width);
		for (auto& l : LinesX) {
			l.resize(DataExt.Height + 1);
		}
		LinesY.resize(DataExt.Width + 1);
		for (auto& l : LinesY) {
			l.resize(DataExt.Height);
		}

		for (const auto& block : *DataExt.Foundations) {
			LinesX[block.X][block.Y] = !LinesX[block.X][block.Y];
			LinesX[block.X][block.Y + 1] = !LinesX[block.X][block.Y + 1];
			LinesY[block.X][block.Y] = !LinesY[block.X][block.Y];
			LinesY[block.X + 1][block.Y] = !LinesY[block.X + 1][block.Y];
		}

		for (size_t y = 0; y < DataExt.Height + 1; ++y) {
			size_t length = 0;
			for (size_t x = 0; x < DataExt.Width; ++x) {
				if (LinesX[x][y]) {
					++length;
					continue;
				}
				if (!length) {
					continue;
				}
				MapCoord start, end;
				start.X = ((x - length) - y) * 30;
				start.Y = ((x - length) + y) * 15;
				end.X = (x - y) * 30;
				end.Y = (x + y) * 15;
				DataExt.LinesToDraw->push_back(std::make_pair(start, end));
				length = 0;
			}
			if (length) {
				MapCoord start, end;
				start.X = ((DataExt.Width - length) - y) * 30;
				start.Y = ((DataExt.Width - length) + y) * 15;
				end.X = (DataExt.Width - y) * 30;
				end.Y = (DataExt.Width + y) * 15;
				DataExt.LinesToDraw->push_back(std::make_pair(start, end));
			}
		}

		for (size_t x = 0; x < DataExt.Width + 1; ++x) {
			size_t length = 0;
			for (size_t y = 0; y < DataExt.Height; ++y) {
				if (LinesY[x][y]) {
					++length;
					continue;
				}
				if (!length) {
					continue;
				}
				MapCoord start, end;
				start.X = (x - (y - length)) * 30;
				start.Y = (x + (y - length)) * 15;
				end.X = (x - y) * 30;
				end.Y = (x + y) * 15;
				DataExt.LinesToDraw->push_back(std::make_pair(start, end));
				length = 0;
			}
			if (length) {
				MapCoord start, end;
				start.X = (x - (DataExt.Height - length)) * 30;
				start.Y = (x + (DataExt.Height - length)) * 15;
				end.X = (x - DataExt.Height) * 30;
				end.Y = (x + DataExt.Height) * 15;
				DataExt.LinesToDraw->push_back(std::make_pair(start, end));
			}
		}
	};

	pThis->UpdateTypeData();
	if (ID) {
		ProcessType(ID);
		return 0;
	}

	CMapDataExt::BuildingDataExts.clear();
	const auto Types = INIMeta::GetRules().GetSectionItems("BuildingTypes");
	//CMapDataExt::BuildingDataExts.resize(Types.size());
	for (auto& Type : Types) {
		ProcessType(Type);
	}
	return 0;
}

DEFINE_HOOK(4A5089, CMapData_UpdateMapFieldData_Structures_CustomFoundation, 6)
{
	GET(int, BuildingIndex, ESI);
	GET_STACK(const int, X, STACK_OFFS(0x16C, 0x104));
	GET_STACK(const int, Y, STACK_OFFS(0x16C, 0x94));


	//if (BuildingIndex >= CMapDataExt::BuildingDataExts.size()) {
	//	return 0x4A57CD;
	//}

	const auto& DataExt = CMapDataExt::BuildingDataExts[BuildingIndex];
	if (!DataExt.IsCustomFoundation()) {
		for (int dy = 0; dy < DataExt.Width; ++dy) {
			for (int dx = 0; dx < DataExt.Height; ++dx) {
				const int x = X + dx;
				const int y = Y + dy;
				auto pCell = CMapData::Instance->GetCellAt(x, y);
				pCell->Structure = R->BX();
				pCell->TypeListIndex = BuildingIndex;
				CMapData::Instance->UpdateMapPreviewAt(x, y);
			}
		}
		return 0x4A57CD;
	}

	for (const auto& block : *DataExt.Foundations) {
		const int x = X + block.Y;
		const int y = Y + block.X;
		auto pCell = CMapData::Instance->GetCellAt(x, y);
		pCell->Structure = R->BX();
		pCell->TypeListIndex = BuildingIndex;
		CMapData::Instance->UpdateMapPreviewAt(x, y);
	}

	return 0x4A57CD;
}