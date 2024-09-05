#pragma once

#include "defines.hpp"
#include "array.hpp"





template<typename dataType>
class Matrix
{
	public:
		
		
		
		
		
		
	private:
		
		Array<Array<dataType>> m_matrix;
		
		
		
		
		
	protected:
		
		void enableBoundaries(uint32 numberOfColumns, uint32 numberOfRows);
		
		
		
		
		
	public:
		
		Matrix();
		Matrix(uint32 numberOfColumns, uint32 numberOfRows);
		Matrix(uint32 numberOfColumns, uint32 numberOfRows, dataType defaultValue);
		~Matrix();
		
		uint32 get_numberOfRows() const;
		uint32 get_numberOfColumns() const;
		
		dataType get_cell(uint32 columnNumber, uint32 rowNumber) const;
		dataType& cell(uint32 columnNumber, uint32 rowNumber);
		Array<dataType> get_row(uint32 rowNumber) const;
		Array<dataType>& get_row(uint32 rowNumber);
		Array<dataType> get_column(uint32 columnNumber) const;
		
		void insert_row(uint32 rowNumber, Array<dataType>& row);
		void insert_column(uint32 columnNumber, Array<dataType>& column);
		
		void append_row(Array<dataType>& row);
		void append_column(Array<dataType>& column);
		
		void erase();
		feedback delete_row(uint32 rowNumber);
		feedback delete_column(uint32 columnNumber);
		
		void fill(dataType fillValue);
		void set_row(uint32 rowNumber, Array<dataType> row);
		void set_column(uint32 columnNumber, Array<dataType> column);
		void set_cell(uint32 columnNumber, uint32 rowNumber, dataType value);
		
		Matrix& operator+=(const Matrix& matrix);
		Matrix operator+(const Matrix& matrix) const;
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/

template<typename dataType>
void Matrix<dataType>::enableBoundaries(uint32 numberOfColumns, uint32 numberOfRows)
{
	if(numberOfRows == 0 && numberOfColumns == 0)
	{
		return;
	}
	
	if(numberOfRows == 0 && numberOfColumns > 0)
	{
		numberOfRows = 1;
	}
	
	if(m_matrix.get_size() < numberOfRows)
	{
		m_matrix.set_size(numberOfRows);
	}
	
	if(m_matrix[0].get_size() > numberOfColumns)
	{
		numberOfColumns = m_matrix[0].get_size();
	}
	
	for(uint32 i = 0; i < m_matrix.get_size(); i++)
	{
		if(m_matrix[i].get_size() < numberOfColumns)
		{
			m_matrix[i].set_size(numberOfColumns);
		}
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

template<typename dataType>
Matrix<dataType>::Matrix()
{
	
}


template<typename dataType>
Matrix<dataType>::Matrix(uint32 numberOfColumns, uint32 numberOfRows)
{
	enableBoundaries(numberOfColumns, numberOfRows);
}


template<typename dataType>
Matrix<dataType>::Matrix(uint32 numberOfColumns, uint32 numberOfRows, dataType defaultValue)
{
	enableBoundaries(numberOfColumns, numberOfRows);
	fill(defaultValue);
}


template<typename dataType>
Matrix<dataType>::~Matrix()
{
	
}







template<typename dataType>
uint32 Matrix<dataType>::get_numberOfRows() const
{
	return(m_matrix.get_size());
}


template<typename dataType>
uint32 Matrix<dataType>::get_numberOfColumns() const
{
	if(m_matrix.get_size() > 0)
	{
		return(m_matrix[0].get_size());
	}
	return(0);
}







template<typename dataType>
dataType Matrix<dataType>::get_cell(uint32 columnNumber, uint32 rowNumber) const
{
	dataType cell = dataType();
	if(rowNumber < m_matrix.get_size())
	{
		if(columnNumber < m_matrix[rowNumber].get_size())
		{
			cell = m_matrix[rowNumber][columnNumber];
		}
	}
	return(cell);
}


template<typename dataType>
dataType& Matrix<dataType>::cell(uint32 columnNumber, uint32 rowNumber)
{
	if(rowNumber < m_matrix.get_size())
	{
		if(columnNumber < m_matrix[rowNumber].get_size())
		{
			return(m_matrix[rowNumber][columnNumber]);
		}
	}
	return((dataType&) *((dataType*) 0));
}


template<typename dataType>
Array<dataType> Matrix<dataType>::get_row(uint32 rowNumber) const
{
	Array<dataType> row;
	if(rowNumber < m_matrix.get_size())
	{
		row = m_matrix[rowNumber];
	}
	return(row);
}


template<typename dataType>
Array<dataType>& Matrix<dataType>::get_row(uint32 rowNumber)
{
	if(rowNumber < m_matrix.get_size())
	{
		return(m_matrix[rowNumber]);
	}
	return((Array<dataType>&) *((Array<dataType>*) 0));
}


template<typename dataType>
Array<dataType> Matrix<dataType>::get_column(uint32 columnNumber) const
{
	Array<dataType> column;
	if(get_numberOfColumns() <= columnNumber)
	{
		return(column);
	}
	
	uint32 numberOfRows = get_numberOfRows();
	for(uint32 i = 0; i < numberOfRows; i++)
	{
		column += m_matrix[i][columnNumber];
	}
	return(column);
}







template<typename dataType>
void Matrix<dataType>::insert_row(uint32 rowNumber, Array<dataType>& row)
{
	bool empty = false;
	if(m_matrix.get_size() == 0)
	{
		empty = true;
	}
	
	enableBoundaries(row.get_size(), 0);
	
	if(row.get_size() < m_matrix[0].get_size())
	{
		row.set_size(m_matrix[0].get_size());
	}
	
	if(empty == false)
	{
		m_matrix.insert(rowNumber, row);
	}
	else
	{
		m_matrix += row;
	}
}


template<typename dataType>
void Matrix<dataType>::insert_column(uint32 columnNumber, Array<dataType>& column)
{
	enableBoundaries(0, column.get_size());
	
	if(column.get_size() < m_matrix.get_size())
	{
		column.set_size(m_matrix.get_size());
	}
	
	for(uint32 i = 0; i < m_matrix.get_size(); i++)
	{
		m_matrix[i].insert(columnNumber, column[i]);
	}
}







template<typename dataType>
void Matrix<dataType>::append_row(Array<dataType>& row)
{
	bool empty = false;
	if(m_matrix.get_size() == 0)
	{
		empty = true;
	}
	
	enableBoundaries(row.get_size(), 0);
	
	if(row.get_size() < m_matrix[0].get_size())
	{
		row.set_size(m_matrix[0].get_size());
	}
	
	if(empty == false)
	{
		m_matrix += row;
	}
	else
	{
		m_matrix[0] = row;
	}
}


template<typename dataType>
void Matrix<dataType>::append_column(Array<dataType>& column)
{
	enableBoundaries(0, column.get_size());
	
	if(column.get_size() < m_matrix.get_size())
	{
		column.set_size(m_matrix.get_size());
	}
	
	for(uint32 i = 0; i < m_matrix.get_size(); i++)
	{
		m_matrix[i] += column[i];
	}
}







template<typename dataType>
void Matrix<dataType>::erase()
{
	m_matrix.erase();
}


template<typename dataType>
feedback Matrix<dataType>::delete_row(uint32 rowNumber)
{
	if(rowNumber < m_matrix.get_size())
	{
		m_matrix.erase(rowNumber);
		return(OK);
	}
	return(FAIL);
}


template<typename dataType>
feedback Matrix<dataType>::delete_column(uint32 columnNumber)
{
	uint32 numberOfRows = get_numberOfRows();
	if(get_numberOfColumns() <= columnNumber || numberOfRows == 0)
	{
		return(FAIL);
	}
	
	for(uint32 i = 0; i < numberOfRows; i++)
	{
		m_matrix[i].erase(columnNumber);
	}
	return(OK);
}







template<typename dataType>
void Matrix<dataType>::fill(dataType fillValue)
{
	for(auto& i: m_matrix)
	{
		for(auto& j: i)
		{
			j = fillValue;
		}
	}
}


template<typename dataType>
void Matrix<dataType>::set_row(uint32 rowNumber, Array<dataType> row)
{
	enableBoundaries(row.get_size(), rowNumber + 1);
	if(row.get_size() < m_matrix[0].get_size())
	{
		row.set_size(m_matrix[0].get_size());
	}
	m_matrix[rowNumber] = row;
}


template<typename dataType>
void Matrix<dataType>::set_column(uint32 columnNumber, Array<dataType> column)
{
	enableBoundaries(columnNumber + 1, column.get_size());
	if(column.get_size() < m_matrix.get_size())
	{
		column.set_size(m_matrix.get_size());
	}
	for(uint32 i = 0; i < m_matrix.get_size(); i++)
	{
		m_matrix[i][columnNumber] = column[i];
	}
}


template<typename dataType>
void Matrix<dataType>::set_cell(uint32 columnNumber, uint32 rowNumber, dataType value)
{
	enableBoundaries(columnNumber + 1, rowNumber + 1);
	m_matrix[rowNumber][columnNumber] = value;
}







template<typename dataType>
Matrix<dataType>& Matrix<dataType>::operator+=(const Matrix& matrix)
{
	const uint32 numberOfRows = matrix.get_numberOfRows();
	for(uint32 i = 0; i < numberOfRows; i++)
	{
		Array row(matrix.get_row(i));
		append_row(row);
	}
	return(*this);
}


template<typename dataType>
Matrix<dataType> Matrix<dataType>::operator+(const Matrix& matrix) const
{
	Matrix matrixReturn;
	
	matrixReturn += *this;
	matrixReturn += matrix;
	
	return(matrixReturn);
}