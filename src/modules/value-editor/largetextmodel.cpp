#include "largetextmodel.h"


ValueEditor::LargeTextWrappingModel::LargeTextWrappingModel(const QString &text, uint chunkSize)
    : m_chunkSize(chunkSize)
{
    m_textRows.reserve(text.size()/chunkSize);

    for (uint chunkIndex=0; chunkIndex < text.size()/chunkSize + 1; chunkIndex++)
    {
        m_textRows.append(text.mid(chunkIndex * chunkSize, chunkSize));
    }
}

QHash<int, QByteArray> ValueEditor::LargeTextWrappingModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole + 1] = "value";
    return roles;
}

int ValueEditor::LargeTextWrappingModel::rowCount(const QModelIndex &parent) const
{
    return m_textRows.size();
}

QVariant ValueEditor::LargeTextWrappingModel::data(const QModelIndex &index, int role) const
{
    if (!isIndexValid(index))
        return QVariant();

    if (role == Qt::UserRole + 1) {
        return m_textRows[index.row()];
    }

    return QVariant();
}

void ValueEditor::LargeTextWrappingModel::cleanUp()
{
    emit beginRemoveRows(QModelIndex(), 0, rowCount());
    m_textRows.clear();
    emit endRemoveRows();
}

QString ValueEditor::LargeTextWrappingModel::getText()
{
    QString result;
    result.reserve(m_textRows.size() * m_chunkSize);

    for(auto textRow : m_textRows)
    {
        result.append(textRow);
    }

    return result;
}

void ValueEditor::LargeTextWrappingModel::setTextChunk(uint row, QString text)
{
    if (0 <= row && row < m_textRows.size()) {
        m_textRows[row] = text;
        emit dataChanged(createIndex(row, 0), createIndex(row, 0));
    }
}

bool ValueEditor::LargeTextWrappingModel::isIndexValid(const QModelIndex &index) const
{
    return 0 <= index.row() && index.row() < rowCount();
}
