#include "Interprocess.h"
#include <windows.h>


void SendEditorMessage(EditorMessage* editorMessage)
{

  OutputDebugStringA(editorMessage->MessageData);




}
