#include <mysql/jdbc.h>

mysqlx::Session AbrirConexion()
{
    mysqlx::Session session("localhost", 3306, "root", "ingresosegresos");
    mysqlx::Schema mydb = session.getSchema("basedatos");
    mysqlx::Table mytable = mydb.getTable("tabla");
}

int main()
{
    mysqlx::Session session = AbrirConexion();
    printf_s("Conexion a la base de datos");
    // Código para trabajar con la conexión
    session.close();
    return 0;
}
