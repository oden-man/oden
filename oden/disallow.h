#pragma once

#ifndef ODEN_DISALLOW
#define ODEN_DISALLOW

// ÉRÉsÅ[ã÷é~!
#define ODEN_NO_COPY_CLASS( classname ) 		\
private: 										\
	classname( const classname& ); 				\
ODEN_NO_ASSIGN_CLASS( classname ) 

// ë„ì¸ã÷é~!
#define ODEN_NO_ASSIGN_CLASS( classname ) 		\
private: 										\
	classname& operator=( const classname& ); 


#endif



