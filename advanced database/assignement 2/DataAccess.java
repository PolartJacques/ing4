package model;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Jean-Michel Busca
 */
public class DataAccess {

  // class fields
  private static final String[] GET_DEPARTMENTS_QUERIES = {
    "select DID, DNAME, DLOC from DEPT",
    "select DID, DNAME, DLOC from DEPT where DLOC = ?",
    "select DID, DNAME, DLOC from DEPT where DNAME = ?",
    "select DID, DNAME, DLOC from DEPT where DNAME = ? and DLOC = ?",
    "select DID, DNAME, DLOC from DEPT where DID = ?",
    "select DID, DNAME, DLOC from DEPT where DID = ? and DLOC = ?",
    "select DID, DNAME, DLOC from DEPT where DID = ? and DNAME = ?",
    "select DID, DNAME, DLOC from DEPT where DID = ? and DNAME = ? and DLOC = ?"
  };

  // instance fields
  private final Connection connection;
  private PreparedStatement getEmployeesPS;
  private PreparedStatement updateSalaryPS;
  private PreparedStatement[] getDepartmentsPSs;

  /**
   * Important note on Prepared Statements:
   * <p>
   * Prepared statements are primarily intended to improve performances. As a
   * side effect, they also prevent SQL injection. For both reasons, <b>you must
   * use</b> prepared statements whenever you can. The only situation you can't
   * use a prepared statement is when you don't know the SQL statement to
   * execute beforehand.
   * <p>
   * For a prepared statement to be efficient:<ul>
   * <li>It has to be created <b>only once</b>. Upon creation, the RDMS parses
   * the statement of the SQL statement associated with the prepared statement
   * and then computes its the execution plan
   * <li> And then executed many times, possibly billions times. The execution
   * plan computed at creation time is re-used again and again, saving
   * processing time
   * </ul>
   * Therefore, a prepared statement must be:<ul>
   * <li>an instance attribute; it cannot be a local variable, beacause such a
   * variable is allocated every time you enter the method that declares it,
   * <li>initialized only once, either: (a) beforehand, in the constructor or
   * (b) at the very last time, in the method using the prepared statement; this
   * is a design pattern called "lazy intialization".
   * </ul>
   *
   * @param url the database to connect to
   * @param login the login to use for the connection
   * @param password the password to use for the connection
   *
   * @throws SQLException if an SQL error occurs
   */
  public DataAccess(String url, String login, String password) throws
      SQLException {

    // connect to the database
    connection = DriverManager.getConnection(url, login, password);
    System.out.println("default autocommit mode : " + connection.getAutoCommit());
    System.out.println("isolation level : " + connection.getTransactionIsolation());
    
    connection.setAutoCommit(false);

  }

  public synchronized List<EmployeeInfo> getEmployees() {

    // create a statement; whatever happens, the try-with-resource construct
    // will close the statement, which in turn will close the result set
    try (Statement statement = connection.createStatement()) {

      // execute the query
      ResultSet result = statement.executeQuery(
          "start transaction; select EID, ENAME, SAL from EMP;");

      // convert the result set to a list
      List<EmployeeInfo> list = new ArrayList<>();
      while (result.next()) {
        if (true) {
          // accessing attributes by rank: 1, 2 & 3
          list.add(new EmployeeInfo(result.getInt(1),
              result.getString(2),
              result.getFloat(3)));
        } else {
          // accesing attributes by name: EID, ENAME & SAL
          list.add(new EmployeeInfo(result.getInt("EID"), result
              .getString("EBAME"), result.getFloat("SAL")));
        }
      }
      
      connection.commit();

      return list;

    } catch(SQLException E) {
    	try {
            System.err.print("Transaction is being rolled back");
            connection.rollback();
          } catch (SQLException excep) {
        	  System.err.print(excep.getMessage());
          }
    }
	return null;

  }

  public synchronized List<EmployeeInfo> getEmployeesPS() {

    // create the prepared statement, if not created yet (lazy initialisation
    // design pattern)
    if (getEmployeesPS == null) {
      try {
		getEmployeesPS = connection.prepareStatement(
		      "start transaction; select EID, ENAME, SAL from EMP;");
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }

    // execute the prepared statement; whatever happens, the try-with-resource
    // construct will close the result set
    try (ResultSet result = getEmployeesPS.executeQuery()) {

      List<EmployeeInfo> list = new ArrayList<>();
      while (result.next()) {
        list.add(new EmployeeInfo(result.getInt(1), result.getString(2),
            result.getFloat(3)));
      }
      
      connection.commit();

      return list;

    } catch(SQLException E) {
    	try {
            System.err.print("Transaction is being rolled back");
            connection.rollback();
          } catch (SQLException excep) {
        	  System.err.print(excep.getMessage());
          }
    }
	return null;

  }

  public synchronized boolean raiseSalary(String job, double amount) {

    Statement statement;
	try {
		statement = connection.createStatement();
		
		// do not forget to enclose string litterals (e.g. job) between single quotes:
	    String query = "start transaction; update EMP set SAL = SAL + " + amount + " where JOB = '" + job + "';";
	    int r;
		try {
			r = statement.executeUpdate(query);
			// at least one tuple should have been updated:
			connection.commit();
		    return r >= 1;
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			connection.rollback();
		}
	} catch (SQLException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
	return false;

  }

  public synchronized boolean raiseSalaryPS(String job, double amount) {

    if (updateSalaryPS == null) {
      try {
		updateSalaryPS = connection.prepareStatement(
		      "start transaction; update EMP set SAL = SAL + ? where JOB = ?;");
			// assign a value to all query parameters ("?")
		    try {
				updateSalaryPS.setDouble(1, amount);
				updateSalaryPS.setString(2, job);
	
				connection.commit();
			    return updateSalaryPS.executeUpdate() >= 1;
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				connection.rollback();
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
	return false;

  }

  public List<DepartmentInfo> getDepartments(Integer id, String name,
      String location) {

    // build the query string
    String query = "start transaction; select DID, DNAME, DLOC from DEPT where true;";
    if (id != null) {
      query += " and DID = " + id;
    }
    if (name != null) {
      query += " and DNAME = '" + name + "'";
    }
    if (location != null) {
      query += " and DLOC = '" + location + "'";
    }

    try (Statement statement = connection.createStatement()) {

      ResultSet result = statement.executeQuery(query);

      List<DepartmentInfo> list = new ArrayList<>();
      while (result.next()) {
        list.add(new DepartmentInfo(result.getInt(1),
            result.getString(2),
            result.getString(3)));
      }
      connection.commit();
      return list;

    } catch (SQLException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
		try {
			connection.rollback();
		} catch (SQLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}
	return null;

  }

  public List<DepartmentInfo> getDepartmentsPS(Integer id, String name,
      String location) {

    // compute the prepared statement key; the key has three bits, one for
    // each parameter: a bit is set to 1 if the parameter is specified
    // and left to 0 if not
    int key = 0;
    key += (id == null ? 0 : 4);
    key += (name == null ? 0 : 2);
    key += (location == null ? 0 : 1);

    if (getDepartmentsPSs == null) {
      getDepartmentsPSs = new PreparedStatement[GET_DEPARTMENTS_QUERIES.length];
    }

    // retrieve the appropriate prepared statement
    PreparedStatement getDepartementsPS = getDepartmentsPSs[key];
    if (getDepartementsPS == null) {
      try {
		getDepartementsPS = connection.prepareStatement(
		      "start transaction;" + GET_DEPARTMENTS_QUERIES[key]);
	  } catch (SQLException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	  }
      getDepartmentsPSs[key] = getDepartementsPS;
    }

    // set the prepared statement's parameters: one for each specified parameter
    int index = 0;
    if (id != null) {
      index += 1;
      try {
		getDepartementsPS.setInt(index, id);
	  } catch (SQLException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	  }
    }
    if (name != null) {
      index += 1;
      try {
		getDepartementsPS.setString(index, name);
	  } catch (SQLException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	  }
    }
    if (location != null) {
      index += 1;
      try {
		getDepartementsPS.setString(index, location);
	  } catch (SQLException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	  }
    }

    try (ResultSet result = getDepartementsPS.executeQuery()) {

      List<DepartmentInfo> list = new ArrayList<>();
      while (result.next()) {
        list.add(new DepartmentInfo(result.getInt(1),
            result.getString(2),
            result.getString(3)));
      }
      connection.commit();
      return list;

    } catch (SQLException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
		try {
			connection.rollback();
		} catch (SQLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}
	return null;

  }

  public List<String> executeQuery(String query) {

    // execute the query using a statement: we cannot use a prepared statement
    // since the query can change from one call to the next
    try (Statement statement = connection.createStatement()) {
      ResultSet result = statement.executeQuery("start transaction;" + query);

      // retrieve the result set's metadata
      ResultSetMetaData metaData = result.getMetaData();

      // list attributes, i.e. column names
      List<String> list = new ArrayList<>();
      String attributes = "| ";
      for (int index = 1; index <= metaData.getColumnCount(); index++) {
        attributes += metaData.getColumnName(index) + " | ";
      }
      list.add(attributes);

      // list tuples
      while (result.next()) {
        String tuple = "| ";
        for (int index = 1; index <= metaData.getColumnCount(); index++) {
          tuple += result.getObject(index) + " | ";
        }
        list.add(tuple);
      }
      
      connection.commit();

      return list;

    } catch (SQLException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
		try {
			connection.rollback();
		} catch (SQLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}
	return null;
  }

  public List<String> executeStatement(String statement) throws SQLException {

    // if the statement is a query
    if (statement.toLowerCase().startsWith("select")) {
      return executeQuery(statement);
    }

    // else we assume it is an update
    Statement jdbcStatement = connection.createStatement();
    int r = jdbcStatement.executeUpdate(statement);

    List<String> list = new ArrayList<>();
    list.add(r + "");

    return list;

  }

  public void close() throws SQLException {
    try {
      if (getEmployeesPS != null) {
        getEmployeesPS.close();
      }
    } catch (SQLException e) {
      // exception is ignored: go on closing other resources
    }

    try {
      if (updateSalaryPS != null) {
        updateSalaryPS.close();
      }
    } catch (SQLException e) {
    }

    if (getDepartmentsPSs != null) {
      for (PreparedStatement ps : getDepartmentsPSs) {
        try {
          if (ps != null) {
            ps.close();
          }
        } catch (SQLException e) {
        }
      }
    }

    connection.close();
  }

}
