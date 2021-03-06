package com.gc.hunter;

import com.gc.hunter.tools.IniEditor;
import com.gc.hunter.tools.SqlConnect;
import com.gc.hunter.xls.ReadActorState;
import com.gc.hunter.xls.ReadMonster;
import com.gc.hunter.xls.ReadSkill;
import com.gc.hunter.xls.base.ReadBase;
import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.text.DateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import org.dom4j.Document;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author Yang.Jin
 */
public class MainFrame extends javax.swing.JFrame {

    public enum ExportType {

        TYPE_CLIENT,
        TYPE_SERVER,
    }

    /**
     * load 载入一个xml文档
     *
     * @return 成功返回Document对象，失败返回null
     * @param uri 文件路径
     */
    public static Document load(String filename) {
        Document document = null;
        try {
            SAXReader saxReader = new SAXReader();
            saxReader.setEncoding("UTF-8");
            document = saxReader.read(new File(filename));
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return document;
    }

    public static void save(Document doc, String filename) {
        try {
            OutputFormat outFmt = OutputFormat.createPrettyPrint();
            outFmt.setEncoding("UTF-8");

            XMLWriter xmlWriter = new XMLWriter(new java.io.FileOutputStream(filename), outFmt);
            xmlWriter.write(doc);
            xmlWriter.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    public static String remakeGeneratorConfig(String tableName, String DomainObjectName) {
        try {
            Document doc = load("MybatisConfig/GeneratorConfig.xml");
            Element root = doc.getRootElement();
            String ret = ReadBase.replace(root.element("context").element("sqlMapGenerator").attributeValue("targetPackage"), ".", "/") + "/";
            List<Element> table = root.element("context").elements("table");
            boolean isFind = false;
            for (Element element : table) {
                System.out.println(element.attributeValue("tableName") + " : " + element.attributeValue("domainObjectName"));
                if (tableName.equals(element.attributeValue("tableName"))) {
                    element.setAttributeValue("domainObjectName", DomainObjectName);
                    isFind = true;
                    break;
                }
            }
            if (!isFind) {
                Element temp = root.element("context").addElement("table");
                temp.addAttribute("tableName", tableName);
                temp.addAttribute("domainObjectName", DomainObjectName);
            }

            save(doc, "MybatisConfig/GeneratorConfig.xml");

            return ret;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    public static void remakeMyBatisDao(String fullPath, String mapper) {
        try {
            String tempMapper = fullPath + mapper + "Mapper.xml";
            Document doc = load("MybatisConfig/MyBatisDao.xml");
            Element root = doc.getRootElement();
            boolean isFind = false;
            List<Element> table = root.element("mappers").elements("mapper");
            for (Element element : table) {
                System.out.println(element.attributeValue("resource"));
                if (element.attributeValue("resource").equals(tempMapper)) {
                    element.setAttributeValue("resource", tempMapper);
                    isFind = true;
                    break;
                }
            }
            if (!isFind) {
                Element temp = root.element("mappers").addElement("mapper");
                temp.addAttribute("resource", tempMapper);
            }
            save(doc, "MybatisConfig/MyBatisDao.xml");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * Creates new form MainFrame
     */
    public MainFrame() {
        initComponents();
    }

    /**
     * This method is called from within the constructor to initialize the form. WARNING: Do NOT modify this code. The content of this method is always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jPanel2 = new javax.swing.JPanel();
        jButton3 = new javax.swing.JButton();
        jScrollPane2 = new javax.swing.JScrollPane();
        jList1 = new javax.swing.JList();
        jButton4 = new javax.swing.JButton();
        jButton5 = new javax.swing.JButton();
        jPanel4 = new javax.swing.JPanel();
        isExportServer = new javax.swing.JCheckBox();
        isExportBeans = new javax.swing.JCheckBox();
        isExportClient = new javax.swing.JCheckBox();
        jCheckBox1 = new javax.swing.JCheckBox();
        isExportErlang = new javax.swing.JCheckBox();
        jButton1 = new javax.swing.JButton();
        jButton6 = new javax.swing.JButton();
        jButton7 = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        jTextArea1 = new javax.swing.JTextArea();
        jButton2 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("数据导出工具");

        jPanel2.setBorder(javax.swing.BorderFactory.createTitledBorder("数据库方式导出"));
        jPanel2.setToolTipText("");
        jPanel2.setFocusable(false);
        jPanel2.setName(""); // NOI18N

        jButton3.setBackground(new java.awt.Color(0, 153, 51));
        jButton3.setFont(new java.awt.Font("黑体", 0, 36)); // NOI18N
        jButton3.setForeground(new java.awt.Color(255, 255, 255));
        jButton3.setText("导出");
        jButton3.setSelected(true);
        jButton3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton3ActionPerformed(evt);
            }
        });

        jList1.setBorder(javax.swing.BorderFactory.createCompoundBorder());
        jButton4ActionPerformed(null) ;
        jList1.addListSelectionListener(new javax.swing.event.ListSelectionListener() {
            public void valueChanged(javax.swing.event.ListSelectionEvent evt) {
                jList1ValueChanged(evt);
            }
        });
        jScrollPane2.setViewportView(jList1);

        jButton4.setText("刷新列表");
        jButton4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton4ActionPerformed(evt);
            }
        });

        jButton5.setForeground(new java.awt.Color(255, 0, 51));
        jButton5.setText("刷新服务器");
        jButton5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton5ActionPerformed(evt);
            }
        });

        jPanel4.setBorder(javax.swing.BorderFactory.createTitledBorder("配置"));

        isExportServer.setText("导入到数据库");

        isExportBeans.setText("产生JavaBean");

        isExportClient.setSelected(true);
        isExportClient.setText("导出到本地文件");
        isExportClient.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                isExportClientActionPerformed(evt);
            }
        });

        jCheckBox1.setText("不导出.h");

        isExportErlang.setText("导出erlang");

        javax.swing.GroupLayout jPanel4Layout = new javax.swing.GroupLayout(jPanel4);
        jPanel4.setLayout(jPanel4Layout);
        jPanel4Layout.setHorizontalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel4Layout.createSequentialGroup()
                        .addComponent(isExportClient)
                        .addGap(0, 76, Short.MAX_VALUE))
                    .addGroup(jPanel4Layout.createSequentialGroup()
                        .addComponent(isExportServer)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jCheckBox1))
                    .addGroup(jPanel4Layout.createSequentialGroup()
                        .addComponent(isExportBeans)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(isExportErlang)))
                .addContainerGap())
        );
        jPanel4Layout.setVerticalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addGap(14, 14, 14)
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(isExportServer)
                    .addComponent(jCheckBox1))
                .addGap(18, 18, 18)
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(isExportBeans)
                    .addComponent(isExportErlang))
                .addGap(18, 18, 18)
                .addComponent(isExportClient)
                .addContainerGap(11, Short.MAX_VALUE))
        );

        jButton1.setText("全部");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        jButton6.setText("客户端");
        jButton6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton6ActionPerformed(evt);
            }
        });

        jButton7.setText("服务器");
        jButton7.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton7ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addComponent(jButton1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jButton6, javax.swing.GroupLayout.PREFERRED_SIZE, 81, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jButton7, javax.swing.GroupLayout.PREFERRED_SIZE, 81, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 327, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(jButton5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(jButton4, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                        .addGap(18, 18, 18)
                        .addComponent(jPanel4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jButton3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel2Layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {jButton1, jButton6, jButton7});

        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jButton1, javax.swing.GroupLayout.PREFERRED_SIZE, 51, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                .addComponent(jButton6)
                                .addComponent(jButton7)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 205, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jPanel4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addGap(17, 17, 17)
                                .addComponent(jButton4, javax.swing.GroupLayout.PREFERRED_SIZE, 46, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(18, 18, 18)
                                .addComponent(jButton5, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jButton3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                .addContainerGap())
        );

        jPanel2Layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {jButton1, jButton6, jButton7});

        jTextArea1.setColumns(20);
        jTextArea1.setRows(5);
        jTextArea1.setDisabledTextColor(new java.awt.Color(0, 51, 51));
        jTextArea1.setEnabled(false);
        jTextArea1.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                jTextArea1PropertyChange(evt);
            }
        });
        jScrollPane1.setViewportView(jTextArea1);

        jButton2.setText("Clear");
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jPanel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jScrollPane1))
                .addContainerGap())
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(jButton2)
                .addGap(33, 33, 33))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 241, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jButton2)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void export(ExportType type) throws Exception {
        switch (type) {
            case TYPE_CLIENT:
                /*if (isActorStateChecked.isSelected()) {
                    append("开始导出角色状态表");
                    ReadActorState actorState = new ReadActorState(isBase64Check.isSelected());
                }*/
                /*if (isActorListChecked.isSelected()) {
                 ReadBase readBase = new ReadBase(isBase64Check.isSelected(), "actor", type);
                 if (readBase != null) {
                 readBase.exportClient();
                 }
                 }
                 if (isMonsterChecked.isSelected()) {
                 append("开始导出怪物列表");
                 ReadMonster monster = new ReadMonster(isBase64Check.isSelected());
                 }
                 if (isSkillChecked.isSelected()) {
                 append("开始导出技能总表");
                 ReadSkill skill = new ReadSkill(isBase64Check.isSelected());
                 }*/
                break;

            case TYPE_SERVER:
                ReadBase readBase = null;
                Object[] curSelectGroup = jList1.getSelectedValues();

                if (curSelectGroup == null || curSelectGroup.length <= 0) {
                    showDialog("警告", "选择一个需要导出的库！");
                    return;
                }

                for (Object tname : curSelectGroup) {
                    try {
                        readBase = new ReadBase(false, tname.toString(), type);
                        String nonExist = readBase.checkExist();
                        if( nonExist != null ) {
                            showDialog("出错了", "sheet 《" + nonExist + "》不存在！！！", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        readBase.parse();
                    } catch (Exception e) {
                        e.printStackTrace(); 
                        showDialog("出错了", "导出失败！！" + e.toString(), JOptionPane.ERROR_MESSAGE);
                        return;
                    }

                    if (curSelectGroup.length == 1) {//只选中一个的时候才会按照选择框的配置设定
                        readBase.isExportServer = isExportServer.isSelected();
                        readBase.isExportClient = isExportClient.isSelected();
                        readBase.isNeedExportBeans = isExportBeans.isSelected();
                    }

                    readBase.isExportErlang = isExportErlang.isSelected();

                    if ( exportType == LOAD_TYPE_SERVER && readBase.isExportServer) {
                        try {
                            IniEditor serverInfo = new IniEditor();
                            serverInfo.load(SqlConnect.ServerName);
                            String[] servers = serverInfo.get("server", "server").split(",");
                            for (String server : servers) {
                                SqlConnect.init(server.trim());
                                readBase.exportSQL();
                                SqlConnect.getInstance().close();
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                            showDialog("警告", "导出[" + tname + "]数据库出错(" + e.toString() + ")！");
                            return;
                        }
                    }

                    if (readBase.isExportErlang) {
                        try {
                            readBase.exportErlangModule();
                        } catch (Exception e) {
                            e.printStackTrace();
                            showDialog("警告", "导出[" + tname + "]数据库出错(" + e.toString() + ")！");
                        }
                    }

                    if ( readBase.isExportClient) {
                        try {
                            readBase.exportDBfile(!jCheckBox1.isSelected());
                        } catch (Exception e) {
                            e.printStackTrace();
                            showDialog("警告", "导出[" + tname + "]本地文件出错(" + e.toString() + ")！");
                            return;
                        }
                    }
                }
                showDialog("恭喜", "选择的项目导出成功！！", JOptionPane.INFORMATION_MESSAGE);
                break;

            default:
                break;
        }
    }

    public void showDialog(String title, String text) {
        JOptionPane.showMessageDialog(this, text, title, JOptionPane.ERROR_MESSAGE);
    }

    public void showDialog(String title, String text, int messageType) {
        JOptionPane.showMessageDialog(this, text, title, messageType);
    }

    public static void append(String s) {
        jTextArea1.append(s);
        jTextArea1.append("\n");
    }
    /**
     * 开始按钮
     *
     * @throws Exception
     */
    public static String exportResourcePath;
    public static String exportCodePath;

    public void mkStartActionPerformed(ExportType type) throws Exception {
        export(type);
    }

    private void jTextArea1PropertyChange(java.beans.PropertyChangeEvent evt) {//GEN-FIRST:event_jTextArea1PropertyChange
    }//GEN-LAST:event_jTextArea1PropertyChange

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
        jTextArea1.setText("");
    }//GEN-LAST:event_jButton2ActionPerformed

    private void jButton5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton5ActionPerformed
//        Object[] options = {"确定", "取消", "帮助"};
//        int response = JOptionPane.showOptionDialog(this, "让在运行中的服务器更新数据，会严重影响服务器的运行，务必要谨慎点击！", "刷新服务器提示", JOptionPane.YES_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[0]);
//        if (response == 0) {
//            showDialog("提示", "等新打开的网页提示更新成功，就可以关闭了。");
//            try {
//                java.net.URI uri = new java.net.URI("http://172.16.2.89:11400/SingSomething/ShowServerInfo?user=jinyang&action=ReloadAllDB");
//                java.awt.Desktop.getDesktop().browse(uri);
//            } catch (Exception iOException) {
//            }
//        } else if (response == 1) {
//            showDialog("提示", "明智的选择！！");
//        } else if (response == 2) {
//            showDialog("提示", "请找阳阳！！");
//        }
    }//GEN-LAST:event_jButton5ActionPerformed

    private void isExportClientActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_isExportClientActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_isExportClientActionPerformed

    private void jButton4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton4ActionPerformed
        jList1.setListData( loadConfig(LOAD_TYPE_CLIENT) );
    }//GEN-LAST:event_jButton4ActionPerformed

    public boolean isTrue (String value) {
        boolean ret = false;
        if(value.equalsIgnoreCase("true"))
            ret = true;
        return ret;
    }
    
    public byte exportType                          = LOAD_TYPE_CLIENT;
    public static final byte LOAD_TYPE_ALL          =       0   ;
    public static final byte LOAD_TYPE_SERVER       =       1   ;
    public static final byte LOAD_TYPE_CLIENT       =       2   ;
    public Vector<String> loadConfig (byte type) {
        exportType = type;
        Vector<String> ret = new Vector<String>();
        IniEditor serverInfo = new IniEditor();
        try {
            serverInfo.load("config.ini");
            for( String value : serverInfo.sectionNames() ) {
                if( type == LOAD_TYPE_ALL && (isTrue( serverInfo.get( value , "exportServer") ) || isTrue( serverInfo.get( value , "exportClient") ) ))
                    ret.add(value);
                else if( type == LOAD_TYPE_SERVER && (isTrue( serverInfo.get( value , "exportServer") )))
                    ret.add(value);
                else if( type == LOAD_TYPE_CLIENT && (isTrue( serverInfo.get( value , "exportClient") )))
                    ret.add(value);
            }
        } catch (IOException ex) {
            Logger.getLogger(MainFrame.class.getName()).log(Level.SEVERE, null, ex);
        }        
        return ret;
    }
    
    private void jList1ValueChanged(javax.swing.event.ListSelectionEvent evt) {//GEN-FIRST:event_jList1ValueChanged
        Object[] curSelectGroup = jList1.getSelectedValues();
        if (curSelectGroup.length == 1) {
            try {
                IniEditor setting = new IniEditor();
                setting.load(ReadBase.ConfigName);
                String group = curSelectGroup[0].toString();

                String tempString = setting.get(group, "exportServer");
                isExportServer.setSelected(tempString != null && tempString.equalsIgnoreCase("true"));
                tempString = setting.get(group, "exportClient");
                isExportClient.setSelected(tempString != null && tempString.equalsIgnoreCase("true"));
                tempString = setting.get(group, "javaBean");
                isExportBeans.setSelected(tempString != null && tempString.equalsIgnoreCase("true"));
            } catch (Exception exception) {
            }
        }
    }//GEN-LAST:event_jList1ValueChanged

    private void jButton3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton3ActionPerformed
        try {
            mkStartActionPerformed(ExportType.TYPE_SERVER);
        } catch (Exception ex) {
            ex.printStackTrace();
            Logger.getLogger(MainFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
    }//GEN-LAST:event_jButton3ActionPerformed

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
        jList1.setListData( loadConfig(LOAD_TYPE_ALL) );
    }//GEN-LAST:event_jButton1ActionPerformed

    private void jButton6ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton6ActionPerformed
        jList1.setListData( loadConfig(LOAD_TYPE_CLIENT) );
    }//GEN-LAST:event_jButton6ActionPerformed

    private void jButton7ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton7ActionPerformed
        jList1.setListData( loadConfig(LOAD_TYPE_SERVER) );
    }//GEN-LAST:event_jButton7ActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(MainFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(MainFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(MainFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(MainFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new MainFrame().setVisible(true);
            }
        });


        try {
            IniEditor serverInfo = new IniEditor();
            serverInfo.load(SqlConnect.ServerName);
            exportResourcePath = serverInfo.get("server", "exportResourcePath");
            exportCodePath = serverInfo.get("server", "exportCodePath");
        } catch (Exception exception) {
            exportResourcePath = "..\\..\\projectHunter\\Resources";
            exportCodePath = "..\\..\\projectHunter\\Classes";
        }
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JCheckBox isExportBeans;
    private javax.swing.JCheckBox isExportClient;
    private javax.swing.JCheckBox isExportErlang;
    private javax.swing.JCheckBox isExportServer;
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton4;
    private javax.swing.JButton jButton5;
    private javax.swing.JButton jButton6;
    private javax.swing.JButton jButton7;
    private javax.swing.JCheckBox jCheckBox1;
    private javax.swing.JList jList1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private static javax.swing.JTextArea jTextArea1;
    // End of variables declaration//GEN-END:variables

    public static String getNowTime() {
        Date now = new Date();
        Calendar cal = Calendar.getInstance();
        DateFormat d1 = DateFormat.getDateTimeInstance(); //默认语言（汉语）下的默认风格（MEDIUM风格，比如：2008-6-16 20:54:53）
        String str1 = d1.format(now);
        return str1;
    }
}
