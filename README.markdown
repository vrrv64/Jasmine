# Jasmine Notes Application

Jasmine is a desktop note-taking application built with C++ and the Qt framework, designed to provide a clean, intuitive interface for organizing and managing notes. It supports folder-based organization, rich text editing, image embedding, and efficient note searching, making it a versatile tool for personal and professional use.

![Screenshot](images/screenshot1.png)

## Features

- **Folder-Based Organization**: Create, delete, and manage folders to categorize notes, with a collapsible folder list for a streamlined interface.
- **Rich Text Editing**: Supports formatting options including bold, italic, underline, strikethrough, text alignment, and color customization.
- **Image and Text File Support**: Drag-and-drop images and text files into notes, with automatic image scaling for optimal display.
- **Search and Filtering**: Real-time note filtering using a custom proxy model to search by title and content.
- **Persistent Storage**: Uses SQLite for reliable data storage, with settings preserved via QSettings for consistent user experience across sessions.
- **Hyperlink Support**: Formats URLs as clickable hyperlinks.

## Installation

### Prerequisites
- Qt 5.12 or higher (with Qt Widgets and SQL modules)
- C++17 compatible compiler (e.g., GCC, MSVC, Clang)
- SQLite (included with Qt's SQL module)

### Build Instructions
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/vrrv64/jasmine.git
   cd jasmine
   ```

2. **Set Up Qt**:
   Ensure Qt is installed and configured in your environment. Set the `QTDIR` environment variable to your Qt installation path if needed.

3. **Build the Project**:
   ```bash
   qmake jasmine.pro
   make
   ```

4. **Run the Application**:
   ```bash
   ./jasmine
   ```

## Usage

1. **Launch Jasmine**: Run the application to access the main interface.
2. **Manage Folders**:
   - Click the "Add Folder" button or use `Ctrl+Shift+N` to create a new folder.
   - Select a folder to view its notes, or hide the folder list using the toggle button.
3. **Manage Notes**:
   - Create a new note with `Ctrl+N` or the "Add Note" button.
   - Edit notes in the rich text editor, with formatting options available in the "Format" menu.
   - Drag-and-drop images or text files into the editor to embed them.
   - Search notes using the search bar above the note list.
4. **Delete Notes/Folders**:
   - Select a note or folder and click the respective delete button or use the context menu.
   - Confirm deletion in the dialog prompt.
5. **Customize Text**:
   - Use `Ctrl+B`, `Ctrl+I`, `Ctrl+U`, or `Ctrl+T` for bold, italic, underline, or strikethrough.
   - Adjust text alignment with `Ctrl+L`, `Ctrl+E`, `Ctrl+R`, or `Ctrl+J`.
   - Change font or colors via the "Format" menu.

## License
Jasmine is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Credits

### Icons
- **Note Special Lineal color icon** by [Freepik](https://www.freepik.com) - [Free vector icon](https://www.freepik.com/icon/note_4021693)
- **Tango icons** from [Wikimedia Commons](https://commons.wikimedia.org/wiki/Tango_icons) - Released into the public domain under CC0 1.0 Universal.
