#include "qirclineedit.h"

QIRCLineEdit::QIRCLineEdit()
{
    connect(this,SIGNAL(textEdited(QString)),this,SLOT(myTextEdited()));
    listindex=0;
}

void QIRCLineEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Up) {
        if(text()==""){
            if(m_Inputlist.empty()){

            }else{
                listindex--;
                setText(m_Inputlist[listindex]);
            }
        }else if(text()!=""){
            if(listindex<=m_Inputlist.count()){
                if(listindex==0){
                    setText(m_Inputlist[0]);
                }else{
                    if(listindex==m_Inputlist.count()) m_Inputlist.append(text());
                    listindex--;
                    setText(m_Inputlist[listindex]);
                }
            }else if(listindex>m_Inputlist.count()){
                if(!m_Inputlist.empty()){
                    m_Inputlist.append(text());
                    setText(m_Inputlist[listindex--]);
                }
            }
        }
    }else if(e->key() == Qt::Key_Down) {
        if(text()==""){
            if(m_Inputlist.empty()){

            }else{
                listindex++;
                setText(m_Inputlist[listindex]);
            }
        }else if(text()!=""){
            if(listindex+1<m_Inputlist.count()){
                listindex++;
                setText(m_Inputlist[listindex]);
            }else if(listindex>=m_Inputlist.count()){

            }

        }
    }else{
        QLineEdit::keyPressEvent(e);
    }

}
void QIRCLineEdit::myTextEdited()
{
    if(m_Inputlist.count()==0){
        listindex=1;
    }else{
        listindex=m_Inputlist.count();
    }
}

void QIRCLineEdit::myclear()
{
    if(text()!=""){
        m_Inputlist.append(text());
        if(m_Inputlist.count()==0){
            listindex=1;
        }else{
            listindex=m_Inputlist.count();
        }
    }
    QLineEdit::clear();
}
