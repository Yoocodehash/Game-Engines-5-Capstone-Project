using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class MatrixDLLTest : MonoBehaviour
{
    [DllImport("Matrix")]
    public static extern void setColumns(int columns_);
    [DllImport("Matrix")]
    public static extern void setRows(int rows_);
    [DllImport("Matrix")]
    public static extern int getColumns();
    [DllImport("Matrix")]
    public static extern int getRows();

    // Start is called before the first frame update
    void MatrixStart()
    {
        setColumns(3);
        setRows(3);

        Debug.Log("Columns: " + getColumns() + "  Rows: " + getRows());
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
