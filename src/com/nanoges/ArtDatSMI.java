package com.nanoges;

public class ArtDatSMI {

	private String ART_CODIGO;
	private String ART_NOMBRE;
	private String ART_FAMILI;
	private String ART_DESUNI;
	private String ART_LOTE1U;
	private String ART_LOTE2U;
	private String ART_TIPIVA;
	private String ART_PVENTA;
	private String ART_TIPMAT;
	private String ART_ENVPRE;
	private String ART_DESCL1;
	private String ART_TIPART;
	private String ART_TASAPV;
	private String ART_PROMOC;

		
		public String getART_CODIGO() {
			return ART_CODIGO;
		}

		public String getART_NOMBRE() {
			return ART_NOMBRE;
		}
		
		public String getART_FAMILI() {
			return ART_FAMILI;
		}
		
		public String getART_DESUNI() {
			return ART_DESUNI;
		}
		
		public String getART_LOTE1U() {
			return ART_LOTE1U;
		}
		
		public String getART_LOTE2U() {
			return ART_LOTE2U;
		}
		
		public String getART_TIPIVA() {
			return ART_TIPIVA;
		}
		
		public String getART_PVENTA() {
			return ART_PVENTA;
		}
		
		public String getART_TIPMAT() {
			return ART_TIPMAT;
		}
		
		public String getART_ENVPRE() {
			return ART_ENVPRE;
		}
		
		public String getART_DESCL1() {
			return ART_DESCL1;
		}
		
		public String getART_TIPART() {
			return ART_TIPART;
		}
		
		public String getART_TASAPV() {
			return ART_TASAPV;
		}
		
		public String getART_PROMOC() {
			return ART_PROMOC;
		}





		public void setArtDatSMI(String CODIGO, String NOMBRE, String FAMILI, String DESUNI, String LOTE1U, String LOTE2U
				, String TIPIVA, String PVENTA, String TIPMAT, String ENVPRE, String DESCL1, String TIPART, String TASAPV
				, String PROMOC) {
			
			this.ART_CODIGO = CODIGO;
			this.ART_NOMBRE = NOMBRE;
			this.ART_FAMILI = FAMILI;
			this.ART_DESUNI = DESUNI;
			this.ART_LOTE1U = LOTE1U;
			this.ART_LOTE2U = LOTE2U;
			this.ART_TIPIVA = TIPIVA;
			this.ART_PVENTA = PVENTA;
			this.ART_TIPMAT = TIPMAT;
			this.ART_ENVPRE = ENVPRE;
			this.ART_DESCL1 = DESCL1;
			this.ART_TIPART = TIPART;
			this.ART_TASAPV = TASAPV;
			this.ART_PROMOC = PROMOC;
		}

		// Will be used by the ArrayAdapter in the ListView
		@Override
		public String toString() {
			return ART_CODIGO + ".-" + ART_NOMBRE;
		}
	}