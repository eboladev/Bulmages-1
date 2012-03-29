package com.nanoges;

public class Articulo {
	private long id=0;
	private String codigocompleto;
	private String desc;
	private String pvp;
	private String iva;
	
	public long getId() {
		return id;
	}

	public String getCodigoCompleto() {
		return codigocompleto;
	}
	
	public String getNom() {
		return desc;
	}
	
	public void setId(long id) {
		this.id = id;
	}

	public String getArticulo() {
		return desc;
	}


	public String getPrecioVenta() {
		return pvp;
	}

	public String getIva() {
		return iva;
	}

	
	public void setArticulo(String codcompleto, String descripcion, String mpvp, String miva) {
		this.codigocompleto = codcompleto;
		this.desc = descripcion;
		this.pvp = mpvp;
		this.iva = miva;
	}

	// Will be used by the ArrayAdapter in the ListView
	@Override
	public String toString() {
		return codigocompleto + ".-" + desc;
	}
}