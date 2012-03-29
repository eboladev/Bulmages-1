package com.nanoges;


public class LPedido {

		private long id;
		private String tablename = "lpedidocliente";
		private String idpedidocliente;
		private String idarticulo;
		private String cantl;
		private String descl;
		
		public long getId() {
			return id;
		}

		public String getIdPedidoCliente() {
			return idpedidocliente;
		}
		public String getDescl() {
			return descl;
		}
		
		public String getCantl() {
			return cantl;
		}
		
		public String getIdArticulo() {
			return idarticulo;
		}
		
		
		public void setId(long id) {
			this.id = id;
		}

		public String getLPedido() {
			return descl;
		}

		public void setLPedido(String midpedidocliente, String midarticulo, String mcantl, String mdescl) {
			this.idpedidocliente = midpedidocliente;
			this.idarticulo = midarticulo;
			this.cantl = mcantl;
			this.descl= mdescl;
		}

		// Will be used by the ArrayAdapter in the ListView
		@Override
		public String toString() {
			return cantl + ".-" + descl;
		}
}