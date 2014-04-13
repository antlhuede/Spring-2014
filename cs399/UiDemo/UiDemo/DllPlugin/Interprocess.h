
const int fill = 1;

enum Messages
{
  DebugConsole,
  InternalError
};

struct EditorMessage
{
  int MessageSize;
  int MessageType;
  char MessageData[fill];
};

const int EditorMessageSize = sizeof(EditorMessage) - fill;

void SendEditorMessage(EditorMessage* message);
