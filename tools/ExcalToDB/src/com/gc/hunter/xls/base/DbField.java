/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.gc.hunter.xls.base;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Yang.Jin
 */
public class DbField {
    
    private String Field ;
    private String Type ;
    
    public DbField(ResultSet rs) {
        try {
            Field = rs.getString( "Field" ) ;
            Type  = rs.getString("Type") ;
        } catch (Exception ex) {
            ex.printStackTrace();
            Logger.getLogger(DbField.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    
    
}
