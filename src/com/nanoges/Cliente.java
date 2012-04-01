package com.nanoges;

public class Cliente {

		private long id=0;
		private String cif;
		private String nom;
		private String longitud;
		private String latitud;
		
		public long getId() {
			return id;
		}

		public String getCif() {
			return cif;
		}
		
		public String getNom() {
			return nom;
		}
		
		public String getLongitud() {
			return longitud;
		}
		
		public String getLatitud() {
			return latitud;
		}
		
		public void setId(long id) {
			this.id = id;
		}

		public String getCliente() {
			return nom;
		}

		public void setCliente(String mcif, String mnom, String longi, String lat) {
			this.cif = mcif;
			this.nom = mnom;
			this.longitud = longi;
			this.latitud = lat;
		}

		// Will be used by the ArrayAdapter in the ListView
		@Override
		public String toString() {
			return cif + ".-" + nom;
		}
	}