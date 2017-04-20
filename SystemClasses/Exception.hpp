#ifndef SYSTEM__EXCEPTION_HPP
#define SYSTEM__EXCEPTION_HPP

#include <string>

namespace System {

// http://stackoverflow.com/a/8152888
class Exception: public std::exception
{
public:
    /** Constructor (C strings).
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller. 
     */
    explicit Exception(const char* message):
      msg_(message)
      {
      }

    explicit Exception(const wchar_t* message):
      msg_("")
      {
      }

    /** Constructor (C++ STL strings).
     *  @param message The error message.
     */
    explicit Exception(const std::string& message):
      msg_(message)
      {}

    /** Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~Exception() throw (){}

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *          is in posession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char* what() const throw (){
       return msg_.c_str();
    }

	String Message() const
	{
		return msg_;
	}

protected:
    /** Error message.
     */
    std::string msg_;
};

} // System

#endif

