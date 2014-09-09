package com.gc.hunter.tools;

import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import com.gc.hunter.MainFrame;
import com.mysql.jdbc.Connection;

public class SqlConnect {

    public static final String ServerName = "server.ini";
    private Connection con;
    private Statement statement;
    private static SqlConnect instance;

    public static SqlConnect getInstance() {
        if (instance == null) {
            try {
                throw new Exception("need init SqlConnect");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return instance;
    }

    public static void init(String serverName) {
        instance = new SqlConnect(serverName);
    }

    private SqlConnect(String serverName) {
        try {
            IniEditor serverInfo = new IniEditor();
            serverInfo.load(ServerName);
            String url = serverInfo.get(serverName, "url");
            String userName = serverInfo.get(serverName, "user");
            String passWord = serverInfo.get(serverName, "password");

            con = (Connection) DriverManager.getConnection("jdbc:mysql://"
                    + url + "?useUnicode=true&characterEncoding=utf8",
                    userName, passWord);
            statement = con.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,
                    ResultSet.CONCUR_UPDATABLE);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public ResultSet querySQL(String sql) throws Exception {
        ResultSet rs = null;
        rs = statement.executeQuery(sql);
        return rs;
    }

    /**
     * 执行sql
     *
     * @param sql
     */
    public void execSQL(String sql)  throws Exception {
        statement.executeUpdate(sql);
    }

    public void close() {
        try {
            statement.close();
            con.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    /**
     * 是否存在表
     *
     * @param table
     * @return
     */
    public boolean isExist(String table) throws Exception {
        ResultSet rs = con.getMetaData().getTables(null, null, table, null);
        if (rs == null) {
            return false;
        }
        rs.last();
        int c = rs.getRow();
        rs.close();
        rs = null;
        if (c > 0) {
            return true;
        }
        return false;
    }

    /**
     * 删除表
     *
     * @param con
     * @param table
     * @throws Exception
     */
    public void delete(String table) throws Exception {
        String sql = "drop table if exists `" + table + "`;";
        execSQL(sql);
    }

    public void rename(String oldtable, String newtable) throws Exception {
        String sql = "ALTER TABLE `"+oldtable+"` RENAME TO  `"+newtable+"` ;";
        execSQL(sql);
    }
    
    /**
     * 删除表
     *
     * @param con
     * @param table
     * @throws Exception
     */
    public void clear(String table) throws Exception {
        String sql = "TRUNCATE TABLE `" + table + "`;";
        execSQL(sql);
    }
}
