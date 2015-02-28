#include "RecordTabHandler.h"


RecordTabHandler::RecordTabHandler()
{
}


RecordTabHandler::~RecordTabHandler()
{
}

void RecordTabHandler::setSharedRecordingConfiguration(SharedRecordingConfiguration recordingConfiguration)
{
	m_recordingConfiguration = recordingConfiguration;
}

LRESULT CALLBACK RecordTabHandler::MessageRouterTab(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RecordTabHandler* pThis = nullptr;

	if (WM_INITDIALOG == uMsg)
	{
		pThis = reinterpret_cast<RecordTabHandler*>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		pThis = reinterpret_cast<RecordTabHandler*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis)
	{
		return pThis->DlgProcTab(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

void RecordTabHandler::onCreate(WPARAM wParam, LPARAM)
{
	Edit_SetText(GetDlgItem(m_hWnd, IDC_HDFACE_EDIT_BOX),			m_recordingConfiguration->at(   HDFace	).getFileName());
	Edit_SetText(GetDlgItem(m_hWnd, IDC_FACE_RAW_EDIT_BOX),			m_recordingConfiguration->at(  FaceRaw	).getFileName());
	Edit_SetText(GetDlgItem(m_hWnd, IDC_FULL_RAW_DEPTH_EDIT_BOX),	m_recordingConfiguration->at(FullDepthRaw).getFileName());

	HWND hdFaceComboBox = GetDlgItem(m_hWnd, IDC_HD_FACE_COMBO_BOX);
	HWND facerawDepthComboBox = GetDlgItem(m_hWnd, IDC_FACE_RAW_DEPTH_COMBO_BOX);
	HWND fullRawDepthCombobox = GetDlgItem(m_hWnd, IDC_FULL_RAW_DEPTH_COMBO_BOX);

	//for (int i = RECORD_FILE_FORMAT_COUNT-1; i >= 0; --i){
	for (int i = 0; i < RECORD_FILE_FORMAT_COUNT; i++){
		LPTSTR fileFormatName = RecordingConfiguration::getFileFormatAsString(static_cast<RecordingFileFormat>(i));
		ComboBox_AddString(hdFaceComboBox, fileFormatName);
		ComboBox_AddString(facerawDepthComboBox, fileFormatName);
		ComboBox_AddString(fullRawDepthCombobox, fileFormatName);
		if (i == 0){
			ComboBox_SetCurSel(hdFaceComboBox, i);
			ComboBox_SetCurSel(facerawDepthComboBox, i);
			ComboBox_SetCurSel(fullRawDepthCombobox, i);
		}
	}
}


LRESULT CALLBACK RecordTabHandler::DlgProcTab(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		m_hWnd = hWnd;
		onCreate(wParam, lParam);
		break;

		// If the titlebar X is clicked, destroy app
	case WM_CLOSE:
		DestroyWindow(hWnd);
		//m_pclFaceRawViewer->stopViewer();
		//m_pclFaceViewer->stopViewer();
		break;

	case WM_DESTROY:
		// Quit the main message pump
		//m_listView.OnDestroy(m_hWnd);
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
	{
		processUIMessage(wParam, lParam);
		break;
	}
	case WM_SIZE:
		//m_listView.OnSize(m_hWnd, LOWORD(lParam), HIWORD(lParam), static_cast<UINT>(wParam));
		//m_listView.OnSize(m_hWnd, LOWORD(lParam), HIWORD(lParam), static_cast<UINT>(wParam));
		break;
	case WM_NOTIFY:
		break;

	}

	return FALSE;
}


void RecordTabHandler::processUIMessage(WPARAM wParam, LPARAM handle)
{

	switch (HIWORD(wParam))
	{
	case CBN_SELCHANGE:
		onSelectionChanged(wParam, handle);
		break;
	case BN_CLICKED:
		onButtonClicked(wParam, handle);
		break;
	case EN_CHANGE:
		onEditBoxeChanged(wParam, handle);
	default:
		break;
	}
}



void RecordTabHandler::onSelectionChanged(WPARAM wParam, LPARAM handle)
{
	int currentSelection = ComboBox_GetCurSel(GetDlgItem(m_hWnd, LOWORD(wParam)));
	switch (LOWORD(wParam))
	{
	case IDC_FACE_RAW_DEPTH_COMBO_BOX:
	{
		m_recordingConfiguration->at(FaceRaw).setFileFormat(static_cast<RecordingFileFormat>(currentSelection));
		break;
	}
	case IDC_FULL_RAW_DEPTH_COMBO_BOX:
		m_recordingConfiguration->at(FullDepthRaw).setFileFormat(static_cast<RecordingFileFormat>(currentSelection));
		break;
	case IDC_HD_FACE_COMBO_BOX:
		m_recordingConfiguration->at(HDFace).setFileFormat(static_cast<RecordingFileFormat>(currentSelection));
		break;
	}
}


void RecordTabHandler::checkRecordingConfigurationPossible()
{
	bool oneEnabled = false;
	bool allValid = true;
	for (int i = 0; i < RECORD_CLOUD_TYPE_COUNT; i++){
		oneEnabled |=	m_recordingConfiguration->at(i).isEnabled();
		allValid &=		m_recordingConfiguration->at(i).isRecordConfigurationValid();
	}
	if (!oneEnabled || !allValid){
		Button_Enable(GetDlgItem(m_hWnd, IDC_RECORD_BUTTON), false);
	}
	else{
		Button_Enable(GetDlgItem(m_hWnd, IDC_RECORD_BUTTON), true);
	}

}


void RecordTabHandler::onButtonClicked(WPARAM wParam, LPARAM handle)
{
	switch (LOWORD(wParam))
	{
	case IDC_RECORD_BUTTON:
		//if (!m_isCloudWritingStarted)
		//{
		//	m_cloudOutputWriter->startWritingClouds();
		//	SetDlgItemText(m_hWnd, IDC_RECORD_BUTTON, L"Stop");
		//}
		//else{
		//	SetDlgItemText(m_hWnd, IDC_RECORD_BUTTON, L"Record");
		//	m_cloudOutputWriter->stopWritingClouds();
		//}
		//m_isCloudWritingStarted = !m_isCloudWritingStarted;
		break;
	case IDC_HD_FACE_CHECKBOX:
		m_recordingConfiguration->at(HDFace).setEnabled(IsDlgButtonChecked(m_hWnd, IDC_HD_FACE_CHECKBOX));
		break;
	case IDC_FACE_RAW_DEPTH_CHECKBOX:
		m_recordingConfiguration->at(FaceRaw).setEnabled(IsDlgButtonChecked(m_hWnd, IDC_FACE_RAW_DEPTH_CHECKBOX));
		break;
	case IDC_FULL_RAW_DEPTH_CHECKBOX:
		m_recordingConfiguration->at(FullDepthRaw).setEnabled(IsDlgButtonChecked(m_hWnd, IDC_FULL_RAW_DEPTH_CHECKBOX));
		break;
	case IDC_BUTTON_CHOOSE_OUTPUT_DIRECTORY:
	{
		WCHAR szDir[MAX_PATH];
		BROWSEINFO bInfo;
		bInfo.hwndOwner = m_hWnd;
		bInfo.pidlRoot = NULL;
		bInfo.pszDisplayName = szDir; // Address of a buffer to receive the display name of the folder selected by the user
		bInfo.lpszTitle = L"Please, select a output folder"; // Title of the dialog
		bInfo.ulFlags = 0;
		bInfo.lpfn = NULL;
		bInfo.lParam = 0;
		bInfo.iImage = -1;

		LPITEMIDLIST lpItem = SHBrowseForFolder(&bInfo);
		if (lpItem != NULL)
		{
			if (SHGetPathFromIDList(lpItem, szDir)){
				OutputDebugString(szDir);
				SetDlgItemText(m_hWnd, IDC_FILE_PATH_EDIT_BOX, szDir);
				for (int i = 0; i < RECORD_CLOUD_TYPE_COUNT; i++){
					m_recordingConfiguration->at(i).setFilePath(szDir);
				}
			}
		}
		break;
	}
	default:
		break;
	}
}



void RecordTabHandler::onEditBoxeChanged(WPARAM wParam, LPARAM handle)
{
	HWND editBoxHandle = GetDlgItem(m_hWnd, LOWORD(wParam));

	//std::vector<wchar_t> buffer(Edit_GetTextLength(editBoxHandle));
	std::vector<wchar_t> buffer(MAX_PATH);
	Edit_GetText(editBoxHandle, buffer.data(), buffer.size());
	switch (LOWORD(wParam)){
	case IDC_FACE_RAW_EDIT_BOX:
		m_recordingConfiguration->at(FaceRaw).setFileName(buffer.data());
		break;
	case IDC_HDFACE_EDIT_BOX:
		m_recordingConfiguration->at(HDFace).setFileName(buffer.data());
		break;
	case IDC_FULL_RAW_DEPTH_EDIT_BOX:
		m_recordingConfiguration->at(FullDepthRaw).setFileName(buffer.data());
		break;
	case IDC_FILE_PATH_EDIT_BOX:
	{
		for (auto& recordConfig : *m_recordingConfiguration){
			recordConfig.setFilePath(buffer.data());
		}
		break;
	}
	default:
		break;
	}
}

void RecordTabHandler::recordConfigurationStatusChanged(RecordCloudType type, bool newState)
{
	switch (type)
	{
	case HDFace:
		Edit_Enable(GetDlgItem(m_hWnd, IDC_HDFACE_EDIT_BOX), newState);
		ComboBox_Enable(GetDlgItem(m_hWnd, IDC_HD_FACE_COMBO_BOX), newState);
		break;
	case FaceRaw:
		Edit_Enable(GetDlgItem(m_hWnd, IDC_FACE_RAW_EDIT_BOX), newState);
		ComboBox_Enable(GetDlgItem(m_hWnd, IDC_FACE_RAW_DEPTH_COMBO_BOX), newState);
		break;
	case FullDepthRaw:
		Edit_Enable(GetDlgItem(m_hWnd, IDC_FULL_RAW_DEPTH_EDIT_BOX), newState);
		ComboBox_Enable(GetDlgItem(m_hWnd, IDC_FULL_RAW_DEPTH_COMBO_BOX), newState);
		break;
	case RECORD_CLOUD_TYPE_COUNT:
		break;
	default:
		break;
	}

	checkRecordingConfigurationPossible();
}

void RecordTabHandler::recordPathChanged(RecordCloudType type)
{
	checkRecordingConfigurationPossible();
}