#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <Build.h>

/*
 * NOTE: This class is stll in beginner stages and still has some issues
 *
 * Current issues:
 *      * Knowing compressed state of build.
 *      * Indicating specific directories for build compression
 *      * Naming of compressed files
 *      * Deletion of compressed files
 *      * Location of 7zip console application (7z)
 *      * Linux code
 *
 *  Some of these issues may be handled outside of this class
 *
 */

/**
* @class compression
* @brief This class receives a build and handles its compression using 7zip
*/
class compression
{
public:
    /**
    * \fn compression(Build*)
    * @brief constructor which assigns a build internally
    */
    compression(Build*);
    /**
    * \fn compress()
    * @brief compress the build using 7zip
    */
    void compress();
    /**
    * \fn compress()
    * @brief decompress the build using 7zip
    */
    void decompress();

private:
    /**
    * @var build
    * @brief build to be compressed
    */
    Build* build;
};

#endif // COMPRESSION_H
