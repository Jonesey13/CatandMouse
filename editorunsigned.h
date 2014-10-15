
#ifndef EDITORUNSIGNED_H
#define EDITORUNSIGNED_H

#include <string>

class Editor;

class EditorUnsigned : public Storable {
public:
    ~EditorUnsigned()=default;
    EditorUnsigned()=default;
    EditorUnsigned(string NewSig,Editor &NewConfig);
    unsigned *Target;
    Editor *editor;
    void ReadFromStream(ifstream &inputstream) override;
    void WriteToStream(ofstream &outputstream) override;




};


#endif //EDITORUNSIGNED_H

