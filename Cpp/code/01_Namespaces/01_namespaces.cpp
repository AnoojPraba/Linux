#include <iostream>

using namespace std;

// Demonstrates namespace basics, nested namespaces, and using declarations.
namespace Geometry
{
    #define UNIT_SQUARE_SIDE 1

    /*****************************************************************************
     * Name: unitSquareArea
     *
     * Description:
     *         Returns the area of a unit square, defined inside the Geometry
     *         namespace.
     *
     * Returns:
     *         The area of a unit square.
     *****************************************************************************/
    int unitSquareArea()
    {
        return UNIT_SQUARE_SIDE * UNIT_SQUARE_SIDE;
    }

    namespace ThreeD
    {
        /*****************************************************************************
         * Name: unitCubeVolume
         *
         * Description:
         *         Returns the volume of a unit cube, defined inside the nested
         *         Geometry::ThreeD namespace.
         *
         * Returns:
         *         The volume of a unit cube.
         *****************************************************************************/
        int unitCubeVolume()
        {
            return UNIT_SQUARE_SIDE * UNIT_SQUARE_SIDE * UNIT_SQUARE_SIDE;
        }
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Calls functions from a namespace and a nested namespace, first with
 *         full qualification, then via a using declaration.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    cout << "Geometry::unitSquareArea() = " << Geometry::unitSquareArea() << "\n";
    cout << "Geometry::ThreeD::unitCubeVolume() = "
              << Geometry::ThreeD::unitCubeVolume() << "\n";

    using Geometry::unitSquareArea;
    cout << "unitSquareArea() via using declaration = " << unitSquareArea() << "\n";
    return 0;
}
