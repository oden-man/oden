#pragma once

#ifndef ODEN_DISALLOW
#define ODEN_DISALLOW

// �R�s�[�֎~!
#define ODEN_NO_COPY_CLASS( classname ) 		\
private: 										\
	classname( const classname& ); 				\
ODEN_NO_ASSIGN_CLASS( classname ) 

// ����֎~!
#define ODEN_NO_ASSIGN_CLASS( classname ) 		\
private: 										\
	classname& operator=( const classname& ); 


#endif



