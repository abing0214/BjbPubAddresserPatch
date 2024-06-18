#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QStringLiteral("��ʯ���ƻ�༭�� - ����ϵͳ - �������޸Ĳ���"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::Process(QString str_htm_file)
{
    QFile f_in(str_htm_file);
    QString str_filename = f_in.fileName();
    if (!f_in.open(QFile::Text | QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream ts_in(&f_in);
    ts_in.setCodec("GB2312");
    QString str_text_in = ts_in.readAll();
    f_in.close();

    QString str_s = QString::fromLocal8Bit("�� </td>\n        </tr>\n        <tr>\n          <td colspan=3 align=right class=fjr>&nbsp;</td>");
    QString str_r = QString::fromLocal8Bit("�� </td>\n        </tr>\n        <tr>\n          <td colspan=3 align=right class=fjr>���й�ʯ���ƻᡷ�༭��</td>\n        "
                    "</tr>\n        <tr>\n          <td colspan=3 align=right class=fjr>�Ͼ��������������1�Ÿ�������1008��</td>");

    //str_text_in.replace(str_s, str_r);
    //str_text_in.replace("<td align=right class=fjr>&nbsp;</td>", QString::fromLocal8Bit("<td align=right class=fjr>�ʱࣺ") + m_str_zipcode + "</td>");

    // use items in json file
    QFile jFile("BjbPubPatchRplc.json");
    jFile.open(QFile::ReadOnly | QFile::Text);
    QByteArray jData = jFile.readAll();

    QJsonParseError jErr;
    QJsonDocument jDoc = QJsonDocument::fromJson(jData, &jErr);
    if (jDoc.isNull())
    {
        // error
        qDebug() << "Error: JsonDocument is NULL.";
    }

    if (jErr.error != QJsonParseError::NoError)
    {
        // error
        qDebug() << jErr.errorString();
    }

    QString strText;
    if (jDoc.isArray())
    {
        QJsonArray jArr = jDoc.array();
        for (int i = 0; i < jArr.size(); i++)
        {
            QJsonValue jVal = jArr.at(i);
            QString str_file = "";
            if (!jVal.toObject()["STR_FILE"].isNull())
            {
                str_file = jVal.toObject()["STR_FILE"].toString();
            }
            QString str_s = jVal.toObject()["STR_S"].toString();
            QString str_r = jVal.toObject()["STR_R"].toString();

            if (!str_file.isEmpty() && str_filename.indexOf(str_file)<0)
            {
                continue;
            }

            qDebug() << str_s;
            qDebug() << str_r;

            strText.append("Original: " + str_s);
            strText.append("\n");
            strText.append("Updated : " + str_r);
            strText.append("\n");

            str_text_in.replace(str_s, str_r);
        }
    }

    ui->te_log->moveCursor(QTextCursor::End);
    ui->te_log->insertPlainText(strText);
    ui->te_log->moveCursor(QTextCursor::End);

    /*
    // small font size
    //           <td nowrap colspan=3 class=dz2>&nbsp;&nbsp;��³ľ���г�����·466������ʯ�Ϳ�������԰��C��<td>
    QString strTextDZ2;
    QStringList txtLines = str_text_in.split("\n");
    for (int i = 0; i < txtLines.size(); i++)
    {
        QString strText = txtLines.at(i);
        if (strText.contains("class=dz2"))
        {
            QString strLine = strText.trimmed();
            int pos1 = QString("<td nowrap colspan=3 class=dz2>&nbsp;&nbsp;").size();
            int dzSize = strLine.size() - pos1 - QString("<td>").size();
            QString strDZ2 = strLine.mid(pos1, dzSize);
            int szLeft = strDZ2.size()/2;
            int szRight = strDZ2.size() - szLeft;
            strDZ2 = strDZ2.left(szLeft) + "<br>" + strDZ2.right(szRight);
            strDZ2 = QString("<td nowrap colspan=3 class=dz2>&nbsp;&nbsp;") + strDZ2 + "</td>";
            strText = QString("           ") + strDZ2;
        }
        strTextDZ2.append(strText + "\n");
    }
    str_text_in = strTextDZ2;
*/

    QFile f_out(str_htm_file);
    if (!f_out.open(QFile::Text | QIODevice::WriteOnly))
    {
        return false;
    }



    QTextStream ts_out(&f_out);
    ts_out.setCodec("GB2312");
    ts_out << str_text_in;
    ts_out.flush();
    f_out.close();

    return true;

}

void MainWindow::on_pushButton_clicked()
{
    QString str_dir = QFileDialog::getExistingDirectory(this, QStringLiteral("ѡ��Ƭ�����ļ���"));
    if (str_dir.isEmpty())
    {
        return;
    }

    ui->te_log->clear();

    ui->le_dir->setText(str_dir);
    m_str_name = ui->le_name->text();
    m_str_addr = ui->le_addr->text();
    m_str_name = QStringLiteral("���й�ʯ���ƻᡷ�༭��");
    m_str_addr = QStringLiteral("�Ͼ��������������1�Ÿ�������1008��");
    m_str_zipcode = ui->le_zipcode->text();

    QDir d(str_dir);
    QStringList filters = QStringList() << "*.htm";
    QFileInfoList fiList = d.entryInfoList(filters, QDir::Files);

    QStringList fileList;
    foreach (QFileInfo fInfo, fiList)
    {
        if (fInfo.absoluteFilePath().contains(QStringLiteral("���б�")))
        {
            fileList << fInfo.absoluteFilePath();
        }
    }

    foreach (QString strFile, fileList)
    {
        ui->te_log->append(QStringLiteral("���ڴ����ļ� ") + QFileInfo(strFile).fileName() + "...\n");
        bool flag = this->Process(strFile);
        if (flag)
        {
            ui->te_log->append(QStringLiteral("<font color=green>�ɹ�</font>"));
        }
        else
        {
            ui->te_log->append(QStringLiteral("<font color=red>ʧ��</font>"));
        }
        ui->te_log->append("\n");
    }

    ui->te_log->append(QStringLiteral("������ϡ�"));
}
